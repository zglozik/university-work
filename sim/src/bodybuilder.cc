
#include	"bodybuilder.hh"

// ----------------------------------------------------------------------
//	BodyBuilder( Viewer &viewer )
// ----------------------------------------------------------------------

BodyBuilder::BodyBuilder( Viewer &viewer ) 
	: viewer(viewer), body(0)
{
	points = new Body::PointStore;
	parts = new Body::PartStore;
	wrists = new Body::WristStore;

	error_str[0] = '\0';
}

// ----------------------------------------------------------------------
//	~BodyBuilder()
// ----------------------------------------------------------------------

BodyBuilder::~BodyBuilder()
{
	delete body;
	delete points;
	delete parts;
	delete wrists;
}

// ----------------------------------------------------------------------
//	add_part( const String &name, double length,
//		   double weight, double centre, bool show_centre )
// ----------------------------------------------------------------------

bool BodyBuilder::add_part( const String &name, double length,
			   double weight, double centre, bool show_centre )
{
	TEST_EXPR( !body && parts );

	tparts[ name ] = TPart( length, weight, centre, show_centre,
			       parts->end() );
	return true;
}

// ----------------------------------------------------------------------
//	add_wrist( const String &name,
//		    const String &name1, Endpoint p1,
//		    const String &name2, Endpoint p2,
//		    const Point &dir )
// ----------------------------------------------------------------------

bool BodyBuilder::add_wrist( const String &name,
			    const String &name1, Endpoint ep1,
			    const String &name2, Endpoint ep2,
			    const Point &dir )
{
	TEST_EXPR( !body );

	bool	ok = true;
	TPartStore::iterator p1 = tparts.find( name1 );
	TPartStore::iterator p2 = tparts.find( name2 );

	if( p1 == tparts.end() || p2 == tparts.end() || p1 == p2 ) {
		ok = false;
		sprintf( error_str,"One of parts %s and %s doesn't exist"
			" in this context or they are the same.\n",
			(const char *) name1, (const char *) name2 );
	} else {
		TPartStore::iterator outside = 
			(*p1).pos == parts->end() ? p1 : p2;
		if( (*outside).pos != parts->end() ) {
			ok = false;
			sprintf( error_str,"Neither of part %s and %s are "
				"outside the body.\n", 
				(const char *) name1, (const char *) name2 );
		} else {
			String inside_name, outside_name;
			TPartStore::iterator inside;
			if( outside == p1 ) {
				inside = p2;
				inside_name = name2;
				outside_name = name1;
			} else {
				inside = p1;
				inside_name = name1;
				outside_name = name2;
			}
			if( parts->is_empty() ) {
				points->push_back( 0 );
				push_tpart( inside_name, (*points)[0], start,
					   *inside );
			}
			if( (*inside).pos == parts->end() ) {
				ok = false;
				sprintf( error_str, "Both of part %s and %s "
					"are outside the body.\n", 
					(const char *) name1,
					(const char *) name2 );
			} else {
				TEST_EXPR( (*inside).pos != parts->end() );
				TEST_EXPR( (*outside).pos == parts->end() );
				
				Endpoint iep, oep;
				if( inside == p1 ) {
					iep = ep1;
					oep = ep2;
				} else {
					iep = ep2;
					oep = ep1;
				}
				push_tpart( outside_name,
					   endpoint(*(*inside).pos, iep), 
					   oep, *outside );
				(*wrists)[ name ] =
					Body::Wrist( *(*p1).pos, *(*p2).pos,
						    dir );
			}
		}
	}
	return ok;
}

// ----------------------------------------------------------------------
//	endpoint( const Body::Part &part, Endpoint ep )
// ----------------------------------------------------------------------

Point &BodyBuilder::endpoint( const Body::Part &part, Endpoint ep )
{
	Point	*p;
	switch( ep ) {
	    case start:
		p = &part.start();
		break;
	    case end:
		p = &part.end();
		break;
	}
	return *p;
}

// ----------------------------------------------------------------------
//	push_tpart( const String &pname, Point &fixed, Endpoint ep,
//			TPart &tpart )
// ----------------------------------------------------------------------

void BodyBuilder::push_tpart( const String &pname, Point &fixed,
			     Endpoint ep, TPart &tpart )
{
	Body::Part part( 0, 0, tpart.centre, tpart.weight, tpart.show_centre );

	points->push_back( fixed + Point(tpart.length,0,0) );
	switch( ep ) {
	    case start:
		part.point1 = &fixed;
		part.point2 = &points->back();
		break;
	    case end:
		part.point2 = &fixed;
		part.point1 = &points->back();
		break;
	}
	(*parts)[ pname ] = part;
	tpart.pos = parts->find( pname );
}

// ----------------------------------------------------------------------
//	init_wrist( const String &name, double angle )
// ----------------------------------------------------------------------

bool BodyBuilder::init_wrist( const String &name, double angle )
{
	bool ok = true;
	Body::WristStore &w = !body ? *wrists : body->wrists();
	Body::WristStore::iterator ref = w.find( name );

	if( ref == w.end() ) {
		ok = false;
		sprintf( error_str, "Wrist %s doesn't exist.\n",
			(const char *) name );
	} else {
		if( !body ) {
			body = new Body( points, parts, wrists, 
					false, viewer );
			points = 0;
			parts = 0;
			wrists = 0;
		}
		body->bend( name, angle );
	}
	return ok;
};

// ----------------------------------------------------------------------
//	init( const String &name, Endpoint ep, 
//		const Point &point, double angle )
// ----------------------------------------------------------------------

bool BodyBuilder::init( const String &name, Endpoint ep, 
		       const Point &point, double angle )
{
	TEST_EXPR( body != NULL );

	bool ok = true;
	TPartStore::iterator ref = tparts.find( name );

	if( ref == tparts.end() || (*ref).pos == body->parts().end() ) {
		ok = false;
		sprintf( error_str, "Part %s is not inside the body.\n",
			(const char *) name );
	} else {
		Point centre = endpoint(*(*ref).pos, ep);
		Point point2 = 
			endpoint(*(*ref).pos, ep == start ? end : start);
		if( centre != point2 ) {
			double deg =  alpha(point2-centre, Point(1,0,0));
			if( !equal(deg,0) && !equal(deg,180) &&
			   alpha(vector_mul(Point(1,0,0),point2-centre),
				 Point(0,-1,0)) < 90 ) {
				deg *= -1;
			}
			body->rotate( -angle - deg, centre );
		}
		body->move_rel( point - endpoint(*(*ref).pos,ep) );
	}
	return ok;
}

// ----------------------------------------------------------------------
//	operator () ( bool show_centre )
// ----------------------------------------------------------------------

Body *BodyBuilder::operator () ( bool show_centre )
{
	TEST_EXPR( body != NULL );

	body->show_centre( show_centre );
	Body *result = body;

	body = 0;
	points = new Body::PointStore;
	parts = new Body::PartStore;
	wrists = new Body::WristStore;

	error_str[0] = '\0';

	return result;
}

// ----------------------------------------------------------------------
/*
int main()
{
	Viewer	viewer;
	BodyBuilder	body_builder(viewer);

	body_builder.add_part( "shin", 0.8, 5, 1/2. );
	body_builder.add_part( "thigh", 0.9, 15, 2/3. );
	body_builder.add_part( "body", 1.2, 45, 3/4. );

	cout << "---\n";
	body_builder.add_wrist( "knee",
			       "shin", BodyBuilder::end,
			       "thigh", BodyBuilder::start,
			       Point(0,1,0) );
	cout << "---\n";
	body_builder.add_wrist( "waist",
			       "thigh", BodyBuilder::end,
			       "body", BodyBuilder::start,
			       Point(0,-1,0) );
	cout << "---\n";
	body_builder.init_wrist( "knee", 90 );
	body_builder.init_wrist( "waist", 135 );
	cout << "---\n";
	body_builder.init( "shin", BodyBuilder::start, Point(0,0,0), -45 );
	cout << "---\n";

	Body *body = body_builder();

	viewer.begin();
	body->draw();
	viewer.end();
	delete body;

	fgetc(stdin);

	return 0;
}
*/
