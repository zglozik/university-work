
#ifndef	_HERMITE_HH_
#define	_HERMITE_HH_

#include	"debug.hh"
#include	"array.hh"

template< class POINT >
class Hermite {

//	typedefs

    public:
	struct Parameter {
	friend	Hermite;
		Parameter() : t(0), point(0), dpoint(0) { }
		Parameter( double t, const POINT &point ) 
			: t(t), point(point), dpoint(0) { }
	    private:
		double	t;
		POINT	point;
		POINT	dpoint;
	};

	typedef	Array<Parameter>	PointStore;

//	ctors & dtor

    public:
	Hermite() : points(0, 10) { };
	Hermite( const PointStore &points, const POINT &d0, const POINT &dn ) 
		: points(points) 
	{	
		TEST_EXPR( points.size() >= 2 );
		this->points.front().dpoint = d0;
		this->points.back().dpoint = dn;
		update_dpoints();
	}

//	opers

    public:
	Hermite	&operator = ( const Hermite &h );
	POINT	operator () ( double t ) const;

//	private data:

    private:
	void	update_dpoints();
	POINT	diff( int index );

	PointStore	points;
};

//	include implementation
#include	"hermite.cc"

#endif // _HERMITE_HH_
