
#ifndef	_VIEWER_HH_
#define	_VIEWER_HH_

#include	"debug.hh"
#include	"vector.hh"
#include	<stdio.h>

typedef	Vector<double>	Point;

#if defined(_SI_VIEWER)

extern "C" {
#include	"roll.h"
}

class Viewer {

//	disable copy operations

	Viewer( const Viewer & );
	Viewer &operator = ( const Viewer & );

//	ctors & dtor

    public:
	Viewer();
	virtual	~Viewer();

//	opers

    public:
	Viewer	&begin();	// to start drawing a frame
	Viewer	&end();		// called when frame is ready

	void	set_width( double pline_width )
	{
		line_width = pline_width;
	}
	Viewer	&init_frames( int ) { return *this; }
	Viewer	&move_to( const Point &p )
	{
		return move_to( p[0], p[1], p[2] );
	}
	Viewer	&move_to( double x, double y, double z ); 
	Viewer	&line_to( const Point &p )
	{
		return line_to( p[0], p[1], p[2] );
	}
	Viewer	&line_to( double x, double y, double z );

//	private data

    private:
	double	ox, oy, oz;
	Model	model;
	double	line_width;
};

#elif defined(_X_VIEWER)

class Viewer {

//	disable copy operations

	Viewer( const Viewer & );
	Viewer &operator = ( const Viewer & );

//	ctors & dtor

    public:
	Viewer();
	virtual	~Viewer();

//	opers

    public:
	Viewer	&begin();	// to start drawing a frame
	Viewer	&end();		// called when frame is ready

	void	set_width( double ) { }
	Viewer	&init_frames( int frame );
	Viewer	&move_to( const Point &p )
	{
		return move_to( p[0], p[1], p[2] );
	}
	Viewer	&move_to( double x, double y, double z ); 
	Viewer	&line_to( const Point &p )
	{
		return line_to( p[0], p[1], p[2] );
	}
	Viewer	&line_to( double x, double y, double z );

//	private data

    private:
	FILE	*server;
};

#else
#error	"You must define _X_VIEWER or _SI_VIEWER."
#endif

#endif // _VIEWER_HH_
