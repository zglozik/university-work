
#include	"Turtle.h"

#define	String	XString
#include	<Xm/DrawingA.h>
#undef		String

#include	<math.h>

#define	D2R(x)		(x * M_PI / 180.0)

Turtle::Turtle( int &argc, char *argv[] )
{
	static XString	fallback[] = { "*width: 300", "*height: 300",
				       NULL };
	Widget		top_level;
	Dimension	width, height;
	Pixel		fore, back;

	top_level = XtVaAppInitialize( &_app, "turtle", NULL, 0, &argc, argv,
				      fallback,
				      XmNallowShellResize,	False,
				      NULL );
	_w = XtVaCreateManagedWidget( "drawing_area", xmDrawingAreaWidgetClass,
				     top_level,
				     XmNresizePolicy,	XmRESIZE_NONE,
				     NULL );
	XtRealizeWidget( top_level );
	XtVaGetValues( _w,
		      XmNwidth,		&width,
		      XmNheight,	&height,
		      XmNforeground,	&fore,
		      XmNbackground,	&back,
		      NULL );
	XtVaSetValues( top_level,
		      XmNminWidth,	width,
		      XmNmaxWidth,	width,
		      XmNminHeight,	height,
		      XmNmaxHeight,	height,
		      NULL );
	_pixmap = XCreatePixmap( XtDisplay(_w), XtWindow(_w), width, height,
				DefaultDepthOfScreen(XtScreen(_w)) );
	_gc = XCreateGC( XtDisplay(_w), XtWindow(_w), 0, NULL );
	XSetForeground( XtDisplay(_w), _gc, back );
	XFillRectangle( XtDisplay(_w), _pixmap, _gc,
		       0, 0, width, height );
	XSetForeground( XtDisplay(_w), _gc, fore );

	_x = _y = 0;
	_direction = 0;
	_draw = false;
}

Turtle::~Turtle()
{
	XFreeGC( XtDisplay(_w), _gc );
	XFreePixmap( XtDisplay(_w), _pixmap );
	XtDestroyWidget( _w );
}

void Turtle::forward( double len )
{
	int newx = int(_x + len * cos( D2R(_direction) ) + 0.5);
	int newy = int(_y - len * sin( D2R(_direction) ) + 0.5);
	if( _draw ) {
		XDrawLine( XtDisplay(_w), _pixmap, _gc,
			  _x, _y, newx, newy );
	}
	_x = newx;
	_y = newy;
}

void Turtle::backward( double len )
{
	forward( -len );
}

void Turtle::left( double angle )
{
	_direction += angle;
}

void Turtle::right( double angle )
{
	_direction -= angle;
}

void Turtle::pen_up()
{
	_draw = false;
}

void Turtle::pen_down()
{
	_draw = true;
}

void Turtle::direction( double angle )
{
	_direction = angle;
}

void Turtle::position( double x, double y )
{
	_x = int(x + 0.5);
	_y = int(y + 0.5);
}
