
#include	<X11/StringDefs.h>
#include	<X11/Intrinsic.h>
#include	<X11/Shell.h>
#include	<assert.h>
#include	<stdio.h>
#include	"image.hh"
#include	"euler.hh"

Display	*Disp;

int main( int argc, char *argv[] )
{
	XtAppContext	app_context;
	Widget		toplevel, main;

	toplevel = XtVaAppInitialize( &app_context, "Euler", NULL, 0,
				     &argc, argv, NULL,
				     NULL );
	if( argc != 2 ) {
		fprintf( stderr, "usage: euler <bitmap>\n" );
		exit( 1 );
	}
	main = XtVaCreateWidget( "main", widgetClass, toplevel,
				XtNwidth,	1,
				XtNheight,	1,
				NULL );
	Disp = XtDisplay( toplevel );
	XtRealizeWidget( toplevel );

	Image	image;
	int err = image.load( XtWindow(main), argv[1] );
	if( err != BitmapSuccess ) {
		fprintf( stderr, "can't load bitmap %s\n", argv[1] );
		exit( 1 );
	}

	XtVaSetValues( main,
		      XtNbackgroundPixmap, Pixmap(image),
		      XtNwidth,		image.width(),
		      XtNheight,	image.height(),
		      NULL );
	XtVaSetValues( toplevel,
		      XtNwidth,		image.width(),
		      XtNheight,	image.height(),
		      XtNmaxWidth,	image.width(),
		      XtNmaxHeight,	image.height(),
		      XtNminWidth,	image.width(),
		      XtNminHeight,	image.height(),
		      NULL );

	CellMatrix	euler( &image );
	printf( "euler number: %d\n", euler.start() );

	XtManageChild( main );
	XtAppMainLoop( app_context );				       

	return 0;
}
