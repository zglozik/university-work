
#include	"viewer.hh"

#include	<stdlib.h>

// ----------------------------------------------------------------------
//	Viewer class
// ----------------------------------------------------------------------

Viewer::Viewer()
{
	if( (server = popen( "xviewer", "w" )) == NULL ) {
		cerr << "can't execute xviewer" << endl;
		exit( 1 );
	}
}

Viewer::~Viewer()
{
	pclose( server );
}

Viewer	&Viewer::init_frames( int frame )
{
	fprintf( server, "init_frames\n" );
	fprintf( server, "%d\n", frame );

	return *this;
}

Viewer	&Viewer::begin()
{
	fprintf( server, "begin\n" );

	return *this;
}

Viewer	&Viewer::end()
{
	fprintf( server, "end\n" );
	fflush( server );

	return *this;
}

Viewer &Viewer::move_to( double x, double y, double z )
{
	fprintf( server, "move_to\n" );
	fprintf( server, "%10g %10g %10g\n", x, y, z );

	return *this;
}

Viewer &Viewer::line_to( double x, double y, double z )
{
	fprintf( server, "line_to\n" );
	fprintf( server, "%10g %10g %10g\n", x, y, z );

	return *this;
}
