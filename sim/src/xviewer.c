
#include	<stdlib.h>
#include	<stdio.h>
#include	<X11/Intrinsic.h>
#include	<Xm/MainW.h>
#include	<Xm/DrawingA.h>
#include	<unistd.h>
#include	<fcntl.h>

#define	SCALEX( x )	(50 * (x) + viewer->width/2)
#define	SCALEZ( z )	(3*viewer->height/4 - 50 * (z))

typedef struct {
	char	buf[100];
	int	pos;
	Boolean	eof;
} Pipe;

typedef struct {
	Pipe		pipe;
	Widget		area;
	Dimension	width;
	Dimension	height;
	double		x, y, z;
	Pixmap		pixmap;
	GC		gc;
	GC		background;
	int		frame;
} Viewer;

static void pipe_open( Pipe *pipe )
{
	pipe->pos = 0;
	pipe->eof = FALSE;
}

static Boolean pipe_read( Pipe *pipe )
{
	int	c;

	if( pipe->eof ) return FALSE;

	c = read( 0, &pipe->buf[pipe->pos], sizeof(char) );
	while( c != 0 && c != -1 && pipe->buf[pipe->pos] != '\n' ) {
		c = read( 0, &pipe->buf[++pipe->pos], sizeof(char) );
	}
	switch( c ) {
	    case 0:
		if( pipe->pos > 0 ) {
			pipe->buf[++pipe->pos] = '\0';
			pipe->pos = 0;
			return TRUE;
		} else {
			pipe->eof = TRUE;
			return FALSE;
		}
	    case -1:
		return FALSE;
	    default:
		pipe->buf[pipe->pos] = '\0';
		pipe->pos = 0;
		return TRUE;
	}
}

static Pixmap change_pixmap( Viewer *viewer );

static void viewer_init( Viewer *viewer, Widget area )
{
	XGCValues	values;

	pipe_open( &viewer->pipe );
	viewer->area = area;
	viewer->width = 0;
	viewer->height = 0;
	viewer->x = 0;
	viewer->y = 0;
	viewer->z = 0;
	viewer->pixmap = 0;
	XtVaGetValues( viewer->area,
		      XmNforeground,	&values.foreground,
		      XmNbackground,	&values.background,
		      NULL );
	viewer->gc = XtGetGC( viewer->area, 
			     GCForeground | GCBackground, 
			     &values );
	values.foreground = values.background;
	viewer->background = XtGetGC( viewer->area, GCForeground, &values );
	viewer->frame = 0;
	change_pixmap( viewer );
}

static Pixmap change_pixmap( Viewer *viewer )
{
	Dimension	w, h;
	Pixmap		new_pixmap;
	Display		*display = XtDisplay(viewer->area);

	XtVaGetValues( viewer->area,
		      XmNwidth,			&w,
		      XmNheight,		&h,
		      NULL );
	new_pixmap = XCreatePixmap( display, XtWindow(viewer->area),
				   w, h,
				   DefaultDepth(display,
						DefaultScreen(display)) );
	XFillRectangle( display, new_pixmap, viewer->background, 
		       0, 0, w, h );
	if( viewer->pixmap ) {
		XCopyArea( display, viewer->pixmap, new_pixmap,
			  viewer->gc,
			  (viewer->width-w) / 2, (viewer->height-h) / 2,
			  w, h, 0, 0 );
		XFreePixmap( display, viewer->pixmap );
	}
	viewer->pixmap = new_pixmap;
	viewer->width = w;
	viewer->height = h;

	return new_pixmap;
}

static void expose_cb( Widget w, XtPointer cld, XtPointer cad )
{
	Viewer	*viewer = (Viewer *) cld;

	if( !viewer->pixmap ) return;

/*	printf( "expose\n" );*/
	XCopyArea( XtDisplay(viewer->area), 
		  viewer->pixmap, XtWindow(viewer->area), viewer->gc,
		  0, 0, viewer->width, viewer->height, 0, 0 );
}

static void resize_cb( Widget w, XtPointer cld, XtPointer cad )
{
	Viewer	*viewer = (Viewer *) cld;

/*	printf( "resize\n" );*/
	change_pixmap( viewer );
	expose_cb( w, cld, NULL );
}

static void read_cb( XtPointer cld, int *source, XtInputId *id )
{
	Viewer	*viewer = (Viewer *) cld;
	Display	*display = XtDisplay(viewer->area);

	while( pipe_read( &viewer->pipe ) ) {
		if( !strcmp(viewer->pipe.buf, "begin") ) {
			char	buf[50];
			XFillRectangle( display, viewer->pixmap, 
				       viewer->background, 
				       0, 0, viewer->width, viewer->height );
			sprintf( buf, "frame: %3d", viewer->frame++ );
			XDrawString( display, viewer->pixmap, viewer->gc,
				5, 15, buf, strlen(buf) );
		} else if( !strcmp(viewer->pipe.buf, "end") ) {
			expose_cb( viewer->area, viewer, NULL );
		} else if( !strcmp(viewer->pipe.buf, "init_frames") ) {
			while( !pipe_read(&viewer->pipe) );
			viewer->frame = atoi(viewer->pipe.buf);
		} else if( !strcmp(viewer->pipe.buf, "move_to") && 
			  viewer->pixmap ) {
			while( !pipe_read(&viewer->pipe) );
			sscanf( viewer->pipe.buf, "%lg %lg %lg",
			       &viewer->x, &viewer->y, &viewer->z );
		} else if( !strcmp(viewer->pipe.buf, "line_to") && 
			  viewer->pixmap ) {
			double	x, y, z;
			
			while( !pipe_read(&viewer->pipe) );
			sscanf( viewer->pipe.buf, "%lg %lg %lg", &x, &y, &z );
			XDrawLine( display, viewer->pixmap, viewer->gc, 
				  SCALEX(viewer->x), SCALEZ(viewer->z),
				  SCALEX(x), SCALEZ(z) );
			viewer->x = x;
			viewer->y = y;
			viewer->z = z;
		} else if( !strcmp(viewer->pipe.buf, "quit") ) {
			exit( 0 );
		} else {
			fprintf( stderr, "error\n" );
			exit( 1 );
		}
	}
	if( viewer->pipe.eof ) exit( 0 );
}

int main( int argc, char *argv[] )
{
	Widget	top_level;
	Widget	main_window;
	Widget	area;
	Viewer	viewer;

	fcntl( 0, F_SETFL, O_NONBLOCK );
	top_level = XtInitialize( "xviewer", "xviewer", NULL, 0, &argc, argv );
	main_window = XtVaCreateManagedWidget( "main_window",
					      xmMainWindowWidgetClass,
					      top_level, NULL );
	area = XtVaCreateManagedWidget( "area",
				       xmDrawingAreaWidgetClass,
				       main_window, NULL );
	XtRealizeWidget( top_level );

	viewer_init( &viewer, area );
	XtAddCallback( area, XmNresizeCallback, resize_cb, &viewer );
	XtAddCallback( area, XmNexposeCallback, expose_cb, &viewer );
	XtAddInput( 0, (XtPointer) XtInputReadMask, read_cb, &viewer );

	XtMainLoop();

	return 0;
}
