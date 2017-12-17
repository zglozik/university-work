
#include	"land.hh"

const Land &Land::draw() const
{
	viewer.set_width( 0.05 );
	viewer.move_to( -10, 0, 0 );
	viewer.line_to( 10, 0, 0 );
	return *this;
}

bool Land::inside( const Point &point ) const
{
	return less( point[2], 0 );
}

bool Land::is_surface( const Point &point ) const
{
	return equal( point[2], 0 );
}

Point Land::normal( const Point &point ) const
{
	TEST_EXPR( is_surface(point) );

	return Point(0,0,1);
}

Point Land::depth( const Point &point ) const
{
	TEST_EXPR( is_surface(point) || inside(point) );

	return Point(0,0,-point[2]);
}
