
#include	"image.hh"
#include	<X11/Xutil.h>

extern Display	*Disp;

/*----------------------------------------------------------------------
 *	load( Drawable d, const char *file )
 *----------------------------------------------------------------------*/

int Image::load( Drawable d, const char *file )
{
	destroy();

	int x_hot, y_hot;
	Pixmap tbitmap;
	int rv =  XReadBitmapFile( Disp, d, file, &_w, &_h, &tbitmap,
				  &x_hot, &y_hot );
	if( rv != BitmapSuccess )
		return rv;
	_image = XGetImage( Disp, tbitmap, 0, 0, _w, _h, AllPlanes, XYPixmap );

	_bitmap = XCreatePixmap( Disp, d, _w, _h,
				DefaultDepth(Disp,DefaultScreen(Disp)) );
	XCopyPlane( Disp, tbitmap, _bitmap,
		   DefaultGC(Disp,DefaultScreen(Disp)),
		   0, 0, _w, _h, 0, 0, 1 );
	XFreePixmap( Disp, tbitmap );

	return rv;
}

/*----------------------------------------------------------------------
 *	get( unsigned x, unsigned y )
 *----------------------------------------------------------------------*/

bool Image::get( unsigned x, unsigned y )
{
	return XGetPixel( _image, x, y );
}

/*----------------------------------------------------------------------
 *	put( unsigned x, unsigned y, bool value )
 *----------------------------------------------------------------------*/

void Image::put( unsigned x, unsigned y, bool value )
{
	XPutPixel( _image, x, y, value );
}

/*----------------------------------------------------------------------
 *	destroy()
 *----------------------------------------------------------------------*/

void Image::destroy()
{
	if( _bitmap ) XFreePixmap( Disp, _bitmap );
	_bitmap = 0;
	if( _image ) XDestroyImage( _image );
	_image = 0;
}
