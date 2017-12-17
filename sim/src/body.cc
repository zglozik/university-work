
#include	"body.hh"

Body::Body( PointStore *points, PartStore *parts, WristStore *wrists,
	   bool show_centre, Viewer &viewer ) :
	   _points(points), _parts(parts), _wrists(wrists), 
	   _show_centre(show_centre), viewer(viewer)
{
	TEST_EXPR( points != NULL && parts != NULL && wrists != NULL );
}

Body::Body( Viewer &viewer ) : _show_centre(false), viewer(viewer)
{
	_points = new PointStore;
	_parts = new PartStore;
	_wrists = new WristStore;
}

Body::Body( const Body &body ) 
	: _show_centre(body._show_centre), viewer(body.viewer)
{
	_points = new PointStore( *body._points );
	_parts = duplicate_parts( body );
	_wrists = duplicate_wrists( body );
}

Body::~Body()
{
	delete _wrists;
	delete _parts;
	delete _points;
}

Body &Body::operator = ( const Body &body )
{
	// viewer won't be changed

	if( &body != this ) {
		delete _parts;
		delete _wrists;
		*_points = *body._points;
		_parts = duplicate_parts( body );
		_wrists = duplicate_wrists( body );
		_show_centre = body._show_centre;
	}

	return *this;
}

const Body &Body::draw() const
{
	viewer.set_width( 0.1 );
	for( PartStore::iterator part = _parts->begin(); 
	    part != _parts->end(); part++ ) {
		viewer.move_to( (*part).start() );
		viewer.line_to( (*part).end() );
	}
	
	return *this;
}

Body &Body::move( const Point &new_centre_of_gravity )
{
	const Point diff = new_centre_of_gravity - centre_of_gravity();

	for( PointStore::iterator p = _points->begin(); 
	    p != _points->end(); p++ ) {
		*p = *p + diff;
	}

	return *this;
}

Body &Body::move_rel( const Point &new_centre_of_gravity )
{
	return move( centre_of_gravity() + new_centre_of_gravity );
}

Point Body::centre_of_gravity() const
{
	double			weight = 0;
	PartStore::iterator	part;
	Point			centre( 0, 0, 0 );

	for( part = _parts->begin(); part != _parts->end(); part++ ) {
		centre += (*part).weight() * (*part).centre_of_gravity();
		weight += (*part).weight();
	}
	centre = (1/weight) * centre;

	return centre;
}

double Body::J( const Point &centre ) const
{
	double	sum = 0.0;

	for( PartStore::iterator p = (*_parts).begin(); 
	    p != (*_parts).end(); p++ ) {
		double r = abs( (*p).centre_of_gravity() - centre );
		sum += r*r * (*p).weight();
	}
	return sum;
}

double Body::weight() const
{
	double			weight = 0;
	PartStore::iterator	part;

	for( part = _parts->begin(); part != _parts->end(); part++ ) {
		weight += (*part).weight();
	}

	return weight;
}

Body &Body::rotate( double degree, const Point &point )
{
	PointTrans trans = 
		PointTrans().transfer( point[0], point[1], point[2] ) *
		PointTrans().rotate( degree, 'y' ) *
		PointTrans().transfer( -point[0], -point[1], -point[2] );

	transform( _points->begin(), _points->end(), _points->begin(),
		  transform_point(trans) );

	return *this;
}

Body &Body::bend( const char *name, double degree )
{
	TEST_EXPR( (*_wrists).find( name ) != (*_wrists).end() );

	Wrist &wrist = (*_wrists)[ name ];

	return bend( wrist.part1, wrist.part2, degree, wrist.direction );
}

double Body::alpha( const char *name ) const
{
	TEST_EXPR( (*_wrists).find( name ) != (*_wrists).end() );

	Wrist &wrist = (*_wrists)[ name ];
	Point *c, *v1, *v2;
	bool connected = is_connected( wrist.part1, wrist.part2, c, v1, v2 );
	TEST_EXPR( connected );

	Point  product = normal( vector_mul( *v1 - *c, *v2 - *c ) );
	double degree = ::alpha( *v1 - *c, *v2 - *c );

	if( wrist.direction == -product ) {
		degree = 360 - degree;
	}

	return degree;
}

double Body::alpha_horiz( const char *part_name ) const
{
	PartStore::iterator part = _parts->find( part_name );
	TEST_EXPR( part != _parts->end() );

	Point horiz( 1, 0, 0 );
	Point part_dir = (*part).end() - (*part).start();
	double a = ::alpha( horiz, part_dir );

	if( !equal(a,0) && !equal(a,180) && part_dir != Point(0) &&
	   ::alpha( vector_mul(horiz, part_dir), Point(0,1,0) ) < 90 ) {
		a = 360-a;
	}
	
	return a;
}

Body &Body::bend( const Part &p1, const Part &p2, double degree, Point axes )
{
	Point	*c, *v1, *v2;	// according to part1 and part2
	bool	connected;

	TEST_EXPR( axes != Point(0) );
	axes = normal( axes );

	connected = is_connected( p1, p2, c, v1, v2 );
	TEST_EXPR( connected );

	Point	product = normal( vector_mul( *v1 - *c, *v2 - *c ) );
	double	old_degree = ::alpha( *v1 - *c, *v2 - *c );

	if( axes == product ) {
		degree -= old_degree;
	} else if( axes == -product ) {
		degree += old_degree;
	} else if( product == 0 ) {
		if( old_degree > 90 ) { // it means that it's equal to 180
			degree -= 180;
		} 
	} else {
		cerr << "sorry, bad axes\n";
		exit( 1 );
	}
	PointTrans trans = 
		PointTrans().transfer( (*c)[0], (*c)[1], (*c)[2] ) *
		PointTrans().rotate( axes[1] < 0 ? -degree : degree, 'y' ) *
		PointTrans().transfer( -(*c)[0], -(*c)[1], -(*c)[2] );

	walk_body( *c, *v2, transform_point(trans) );

	return *this;
}

Body &Body::bend_rel( const char *name, double degree )
{
	TEST_EXPR( (*_wrists).find( name ) != (*_wrists).end() );

	Wrist &wrist = (*_wrists)[ name ];
	Point *c, *v1, *v2;
	bool connected = is_connected( wrist.part1, wrist.part2, c, v1, v2 );
	TEST_EXPR( connected );

	PointTrans trans = 
		PointTrans().transfer( (*c)[0], (*c)[1], (*c)[2] ) *
		PointTrans().rotate( wrist.direction[1] < 0 ?
				    -degree : degree, 'y' ) *
		PointTrans().transfer( -(*c)[0], -(*c)[1], -(*c)[2] );

	walk_body( *c, *v2, transform_point(trans) );

	return *this;
}

Body::PartStore *Body::duplicate_parts( const Body &body ) const
{
	PartStore *parts = new PartStore;

	for( PartStore::iterator old_part = body._parts->begin();
	    old_part != body._parts->end(); old_part++ ) {
		Point *p1 = point_ref( body, (*old_part).start() );
		Point *p2 = point_ref( body, (*old_part).end() );
		(*parts)[ body._parts->key(old_part) ] = 
			Part( p1, p2, (*old_part).centre, 
			     (*old_part).weight(), (*old_part).show_centre() );
	}
	
	return parts;
}

Body::WristStore *Body::duplicate_wrists( const Body &body ) const
{
	WristStore *wrists = new WristStore( *body._wrists );

	for( WristStore::iterator old_wrist = body._wrists->begin(),
	    new_wrist = wrists->begin();
	    old_wrist != body._wrists->end(); old_wrist++, new_wrist++ ) {
		Point *p[4];

		p[0] = point_ref( body, (*old_wrist).part1.start() );
		p[1] = point_ref( body, (*old_wrist).part1.end() );
		p[2] = point_ref( body, (*old_wrist).part2.start() );
		p[3] = point_ref( body, (*old_wrist).part2.end() );
		(*new_wrist).part1.point1 = p[0];
		(*new_wrist).part1.point2 = p[1];
		(*new_wrist).part2.point1 = p[2];
		(*new_wrist).part2.point2 = p[3];
	}
	
	return wrists;
}

Point *Body::point_ref( const Body &body, const Point &point ) const
{
	PointStore::iterator pos =
		find_ref( body._points->begin(), body._points->end(), 
			 point );
	return pos == body._points->end() ? 
		0 : &*( _points->begin() + (pos - body._points->begin()) );
}

void Body::walk_body( Point &exclude, Point &start, const transform_point &op )
{
	start = op( start );
	List<Point *> list = neighbours( start );

	for( List<Point *>::iterator i = list.begin(); i != list.end(); i++ ) {
		if( *i != &exclude ) {
			walk_body( start, **i, op );
		}
	}
}

List<Point *> Body::neighbours( const Point &point ) const
{
	List<Point *>	list;

	for( PartStore::iterator i = _parts->begin(); 
	    i != _parts->end(); i++ ) {
		if( &(*i).start() == &point ) {
			list.push_back( &(*i).end() );
		} else if( &(*i).end() == &point ) {
			list.push_back( &(*i).start() );
		}
	}

	return list;
}

bool Body::is_connected( const Part &p1, const Part &p2,
			Point *&c, Point *&v1, Point *&v2 ) const
{
	bool	result = true;

	if( &p1.start() == &p2.start() ) {
		c = &p1.start();
		v1 = &p1.end();
		v2 = &p2.end();
	} else if( &p1.start() == &p2.end() ) {
		c = &p1.start();
		v1 = &p1.end();
		v2 = &p2.start();
	} else if( &p1.end() == &p2.start() ) {
		c = &p1.end();
		v1 = &p1.start();
		v2 = &p2.end();
	} else if( &p1.end() == &p2.end() ) {
		c = &p1.end();
		v1 = &p1.start();
		v2 = &p2.start();
	} else {
		result = false;
	}

	return result;
}

bool operator == ( const Body &b1, const Body &b2 )
{
	Body::PointStore &c1 = *b1._points;
	Body::PointStore &c2 = *b2._points;

	if( c1.size() != c2.size() ) return false;

	Body::PointStore::iterator i1 = c1.begin();
	Body::PointStore::iterator i2 = c2.begin();
	bool	ok = true;
	while( i1 != c1.end() && ok ) {
		ok = *i1++ == *i2++;
	}
	return ok;
}


ostream &operator << ( ostream &s, const Body &body )
{
	for( Body::PartStore::iterator i = body._parts->begin();
	    i != body._parts->end(); i++ ) {
		if( i != body._parts->begin() ) s << '\t';
		s << body.alpha_horiz(body._parts->key(i)) << '\t';
		s << (*i).centre_of_gravity() << '\t';
	}
	for( Body::WristStore::iterator w = body.wrists().begin();
	    w != body.wrists().end(); w++ ) {
		s << body.alpha(body.wrists().key(w)) << '\t';
		Point *c, *v1, *v2;
		bool connected = body.is_connected( (*w).part1, (*w).part2,
						   c, v1, v2 );
		TEST_EXPR( connected );
		s << *c << '\t';
	}
	return s;
}

// ----------------------------------------------------------------------
/*
#include	<unistd.h>

int main()
{
	Body::PointStore	*points = new Body::PointStore;
	Body::PartStore		*parts = new Body::PartStore;
	Body::WristStore	*wrists = new Body::WristStore;
	Viewer	viewer;

	points->push_back( Point(0,0,0) );
	points->push_back( Point(1,0,1) );
	points->push_back( Point(0,0,2) );
	points->push_back( Point(0,0,4) );

	(*parts)["shin"] = Body::Part(&(*points)[0], &(*points)[1], 1/2., 2.);
	(*parts)["tigh"] = Body::Part(&(*points)[1], &(*points)[2], 1/2., 2.);
	(*parts)["body"] = Body::Part(&(*points)[2], &(*points)[3], 1/2., 0.);

	(*wrists)["knee"] = 
		Body::Wrist( *parts->find("shin"), *parts->find("tigh"),
			    Point(0,1,0) );
	(*wrists)["waist"] = 
		Body::Wrist( *parts->find("tigh"), *parts->find("body"),
			    Point(0,-1,0) );

	Body	body( points, parts, wrists, viewer );
	Body	body2( viewer );
	Body	body3( viewer );

	cout << body.alpha( "knee" ) << endl;
	cout << body.alpha( "waist" ) << endl;

	cout << body.centre_of_gravity();
	cout << body.weight() << endl;
	cout << "---\n";

	body2 = body;
	cout << "body2:\n";
	cout << body2.move( body2.centre_of_gravity() + Point(1,0,-1) );
	cout << "---\n";
	cout << "body2:\n" << body2 << "---\n";

	body3 = body;
	cout << "body3 (rot):\n";
	cout << body3.rotate( -90, body3.centre_of_gravity() );
	
	viewer.begin(); 
	cout << "body3 (bend):\n";
	cout << body3.bend( "knee", 135 ).bend_rel( "waist", -45 );
	

	cout << "---" << endl;
//	body.draw();
//	body2.draw();
	body3.draw();
	viewer.end();
	fgetc( stdin );

	return 0;
}
*/
