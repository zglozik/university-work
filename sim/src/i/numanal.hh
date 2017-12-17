
#ifndef	_NUMANAL_HH_
#define	_NUMANAL_HH_

#include	"vector.hh"

typedef	Vector<double>	Point;

extern double find_intersection( const Point &c, const Point &p0,
				const Point &n, const Point &p );
extern Point find_line_intersection( const Point &n1, const Point &p1,
				    const Point &n2, const Point &p2 );

#endif // _NUMANAL_HH_
