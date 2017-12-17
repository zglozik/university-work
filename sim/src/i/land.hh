
#ifndef	_LAND_HH_
#define	_LAND_HH_

#include	"debug.hh"
#include	"vector.hh"
#include	"viewer.hh"

typedef	Vector<double>	Point;

class Land {

//	ctors & dtor

    public:
	Land( Viewer &viewer ) : viewer(viewer) { }

//	opers

    public:

	Land	&operator = ( const Land & ) { return *this; }

	const Land &draw() const;

	bool	inside( const Point &point ) const;
	bool	is_surface( const Point &point ) const;
	Point	normal( const Point &point ) const;
	Point	depth( const Point &point ) const;

//	private data

    private:
	Viewer	&viewer;
};

#endif // _LAND_HH_
