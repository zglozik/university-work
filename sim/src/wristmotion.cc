
#include	"wristmotion.hh"

typedef	Hermite<double>	Spline;
	
// ----------------------------------------------------------------------
//	Segment
// ----------------------------------------------------------------------

struct Segment {
    friend WristMotion;
	Segment( double start, double end )
		: start(start), end(end) { }
	virtual	bool evaluate( double t, double &result ) = 0;
    private:
	Segment( const Segment & );
	Segment &operator = ( const Segment & );

	double	start;
	double	end;
};

// ----------------------------------------------------------------------
//	ConstantSegment
// ----------------------------------------------------------------------

struct ConstantSegment : public Segment {
	ConstantSegment( double start, double end, double value ) 
		: Segment(start, end), value(value) { }
	bool evaluate( double, double &result )
	{
		result = value;
		return true;
	}
    private:
	double	value;
};

// ----------------------------------------------------------------------
//	SplineSegment
// ----------------------------------------------------------------------

struct SplineSegment : public Segment {
	SplineSegment( double start, double end, Spline *spline ) 
		: Segment(start, end), spline(spline) { }
	virtual ~SplineSegment() { delete spline; }
	bool evaluate( double t, double &result )
	{
		result = (*spline)( t );
		return true;
	}
    private:
	Spline	*spline;
};

// ----------------------------------------------------------------------
//	ExpressionSegment
// ----------------------------------------------------------------------

struct ExpressionSegment : public Segment {
	ExpressionSegment( double start, double end, 
			  SymbolTable &st, Expression *e )
		: Segment(start, end), st(st), expression(e) { }
	virtual ~ExpressionSegment() { delete expression; }
	bool evaluate( double t, double &result )
	{
		st[ "t" ] = t;
		return expression->evaluate( result );
	}
    private:
	SymbolTable	&st;
	Expression	*expression;
};

// ----------------------------------------------------------------------
//	WristMotion()
// ----------------------------------------------------------------------

WristMotion::WristMotion() : done(false)
{
}

// ----------------------------------------------------------------------
//	~WristMotion()
// ----------------------------------------------------------------------

WristMotion::~WristMotion()
{
	destroy_segments();
}

// ----------------------------------------------------------------------
//	start( double t0 )
// ----------------------------------------------------------------------

void WristMotion::start( double t0 )
{
	destroy_segments();
	empty( tmp_points );
	this->t0 = t0;
	done = false;
}

// ----------------------------------------------------------------------
//	add_point( double t, Expression *e )
// ----------------------------------------------------------------------

bool WristMotion::add_point( double t, Expression *e )
{
	TEST_EXPR( !done );
	TEST_EXPR( e != NULL );
	TEST_EXPR( t > t0 || 
		  (t == t0 && segments.is_empty() && tmp_points.is_empty()) );

	st[ "t" ] = t;
	double result;
	bool ok = e->evaluate( result );
	delete e;

	if( ok ) {
		t0 = t;
		tmp_points.push_back( make_pair(t, result) );
	}
	return ok;
}

// ----------------------------------------------------------------------
//	add_range( double t0, double t1, Expression *e )
// ----------------------------------------------------------------------

bool WristMotion::add_range( double t0, double t1, Expression *e )
{
	TEST_EXPR( !done );
	TEST_EXPR( t1 > t0 && t0 >= this->t0 );

	bool	ok = true;
	double	result;
	if( this->t0 != t0 ) {
		st[ "t" ] = t0;
		ok = e != 0 && e->evaluate( result );
		if( ok ) {
			tmp_points.push_back( make_pair(t0, result) );
			ok = make_spline( tmp_points.front().first(), t0 );
		}
	} else if( tmp_points.size() > 1 ) {
		ok = make_spline( tmp_points.front().first(), t0 );
	} else if( tmp_points.size() == 1 && segments.is_empty() ) {
		Expression *e = new Number( tmp_points.front().second() );
		ok = make_expression( t0, t0, e );
		if( !ok ) delete e;
	}
	empty( tmp_points );

	if( !e ) {
		ok = ok && t0 == this->t0 && segments.size() > 0 &&
			make_constant( t0, t1 );
	} else {
		ok = ok && make_expression( t0, t1, e );
		if( !ok ) delete e;
	}
	if( ok ) {
		TEST_EXPR( segments.size() > 0 );
		ok = segments.back()->evaluate( t1, result );
		this->t0 = t1;
		tmp_points.push_back( make_pair(t1, result) );
	}
						
	return ok;
}

// ----------------------------------------------------------------------
//	make_spline( double t0, double t1 )
// ----------------------------------------------------------------------

bool WristMotion::make_spline( double t0, double t1 )
{
	Spline::PointStore	points( tmp_points.size() );

	Spline::PointStore::iterator		point;
	List< Pair<double,double> >::iterator	tpoint;
	for(  point = points.begin(), tpoint = tmp_points.begin(); 
	    point != points.end(); point++, tpoint++ ) {
		*point = Spline::Parameter( (*tpoint).first(),
					   (*tpoint).second() );
	}
	PointStore::iterator	p0, p1, pn_1, pn;
	p0 = p1 = tmp_points.begin();
	p1++;
	pn = pn_1 = tmp_points.end();
	pn--;
	pn_1--; pn_1--;

	Spline *spline = new Spline( points, 0, 0 );
	Segment *segment = new SplineSegment( t0, t1, spline );
	segments.push_back( segment );

	return true;
}

// ----------------------------------------------------------------------
//	make_constant( double t0, double t1 )
// ----------------------------------------------------------------------

bool WristMotion::make_constant( double t0, double t1 )
{
	TEST_EXPR( segments.size() > 0 );
	
	double result;
	bool ok = segments.back()->evaluate( t0, result );
	if( ok ) {
		Segment *segment = new ConstantSegment( t0, t1, result );
		segments.push_back( segment );
	}

	return ok;
}

// ----------------------------------------------------------------------
//	make_expression( double t0, double t1, Expression *e )
// ----------------------------------------------------------------------

bool WristMotion::make_expression( double t0, double t1, Expression *e )
{
	TEST_EXPR( e != NULL );

	Segment *segment = new ExpressionSegment( t0, t1, st, e );
	segments.push_back( segment );

	return true;
}

// ----------------------------------------------------------------------
//	end()
// ----------------------------------------------------------------------

bool WristMotion::end()
{
	TEST_EXPR( segments.size() > 0 || tmp_points.size() > 0 );

	bool ok;
	ok = tmp_points.size() < 2 || make_spline( tmp_points.front().first(),
						  tmp_points.back().first() );
	if( ok && segments.is_empty() ) {
		Expression *e = new Number( tmp_points.front().second() );
		ok = make_expression( tmp_points.front().first(),
				     tmp_points.front().first(), e );
		if( !ok ) delete e;
	}
	if( ok ) {
		TEST_EXPR( segments.size() > 0 );
		ok = ok && make_constant( segments.back()->end, -1 );
	}
	empty( tmp_points );
	
	done = true;
	return ok;
}

// ----------------------------------------------------------------------
//	evaluate( double t, double &result )
// ----------------------------------------------------------------------

bool WristMotion::evaluate( double t, double &result )
{
	TEST_EXPR( done && segments.size() > 0 );
	TEST_EXPR( segments.front()->start <= t );

	SegmentStore::iterator	s;
	for( s = segments.begin(); s!=segments.end() && (*s)->start<=t; s++ );
	--s;
	bool ok = (*s)->evaluate( t, result );
	
	return ok;
}

// ----------------------------------------------------------------------
//	WristMotion::last_t()
// ----------------------------------------------------------------------

double WristMotion::last_t()
{
	TEST_EXPR( done && segments.size() > 0 );
	
	return segments.back()->start;
}

// ----------------------------------------------------------------------
//	destroy_segments()
// ----------------------------------------------------------------------

void WristMotion::destroy_segments()
{
	while( !segments.is_empty() ) {
		delete *segments.begin();
		segments.destroy( segments.begin() );
	}
}

// ----------------------------------------------------------------------
/*
#include	"bodybuilder.hh"
#include	<unistd.h>
#include	<stdlib.h>

int main()
{
	Viewer		viewer;
	BodyBuilder	body_builder(viewer);

	body_builder.add_part( "shin", 0.8, 5, 1/2. );
	body_builder.add_part( "thigh", 0.9, 15, 2/3. );
	body_builder.add_part( "body", 1.2, 45, 3/4. );

	body_builder.add_wrist( "knee",
			       "shin", BodyBuilder::end,
			       "thigh", BodyBuilder::start,
			       Point(0,1,0) );
	body_builder.add_wrist( "waist",
			       "thigh", BodyBuilder::end,
			       "body", BodyBuilder::start,
			       Point(0,-1,0) );

	Array<WristMotion>	wrist_motions(2);

	bool ok;
	double result;
	wrist_motions[0].start( 0 );
	ok = wrist_motions[0].add_point( 0, new Number(45) );
	ok = ok && wrist_motions[0].add_point( 0.5, new Number(90));
	ok = ok && wrist_motions[0].add_point( 1, new Number(100));
	ok = ok && wrist_motions[0].end();
	ok = ok && wrist_motions[0].evaluate( 0, result );
	body_builder.init_wrist( "knee", result );

	wrist_motions[1].start( 0 );
	ok = wrist_motions[1].add_point( 0, new Number(90) );
	ok = ok && wrist_motions[1].add_point( 0.5, new Number(150) );
	ok = ok && wrist_motions[1].add_point( 1, new Number(170) );
	ok = ok && wrist_motions[1].end();
	ok = ok && wrist_motions[1].evaluate( 0, result );
	body_builder.init_wrist( "waist", result );

	body_builder.init( "shin", BodyBuilder::start, Point(0,0,0), -60 );
	Body *body = body_builder();

	God	god( *body, Land(viewer) );
	delete body;

	sleep( 5 );
	for( double t = 0; t <= 10; t += 0.01 ) {
		God::Metamorphoses motions(2);
		bool	ok;
		double	value;

		ok = wrist_motions[0].evaluate( t, value );
		motions[0] = God::Motion( "knee", value );
		ok = wrist_motions[1].evaluate( t, value );
		motions[1] = God::Motion( "waist", value );

		viewer.begin();
		god.land().draw();
		god.body().draw();
		viewer.end();
		god.time_elapsed( 0.01, motions );
		usleep( 10 );
	}
	fgetc( stdin );

	return 0;
}
*/
