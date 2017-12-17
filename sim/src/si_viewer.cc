
#include	"viewer.hh"

#include	<stdlib.h>

// ----------------------------------------------------------------------
//	Viewer class
// ----------------------------------------------------------------------

Viewer::Viewer() : model(0), line_width(1)
{
}

Viewer::~Viewer()
{
}

Viewer &Viewer::begin()
{
	model = mdlCreate();

	return *this;
}

Viewer &Viewer::end()
{
	TEST_EXPR( model != NULL );

	mdlSave( model, stdout );
	mdlFree( model );

	return *this;
}

Viewer &Viewer::move_to( double x, double y, double z )
{
	TEST_EXPR( model != NULL );

	ox = x;
	oy = y;
	oz = z;

	return *this;
}

Viewer &Viewer::line_to( double x, double y, double z )
{
	TEST_EXPR( model != NULL );

	Color color = { 1, 1, 1, 1/2., 1/2., 1/2., 1/4., 1/4., 1/4. };

	mdlAddRoll( model, "roll", ox, oz, -oy, x, z, -y, line_width, &color );
	move_to( x, y, z );

	return *this;
}

// ----------------------------------------------------------------------
/*
#include	<unistd.h>

int main()
{
	Viewer	viewer;

	viewer.begin();
	viewer.move_to( 0, 0, 0 );
	viewer.line_to( 1, 1, 0 );
	viewer.move_to( 0, 0, 0 );
	viewer.line_to( 2, 0, 0 );
	viewer.end();
	return 0;
}
*/
