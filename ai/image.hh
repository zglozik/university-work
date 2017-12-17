
#ifndef	IMAGE_HH
#define	IMAGE_HH

#include	<X11/Xlib.h>

class Image {

    private:
	Image( const Image & );
	Image &operator = ( const Image & );

    public:
	Image() : _bitmap(0), _image(0) { }
	~Image() { destroy(); }

	int load( Drawable d, const char *file );
	bool get( unsigned x, unsigned y );
	void put( unsigned x, unsigned y, bool value = true );

	unsigned width() { return _w; }
	unsigned height() { return _h; }
	operator Pixmap () { return _bitmap; }

    private:
	void destroy();

	Pixmap		_bitmap;
	XImage		*_image;
	unsigned	_w, _h;

};

#endif /* IMAGE_HH */
