

template< class POINT >
Hermite<POINT> &Hermite<POINT>::operator = ( const Hermite &h )
{
	if( this != &h ) {
		points = h.points;
	}
	return *this;	
}

template< class POINT >
POINT Hermite<POINT>::operator () ( double t ) const
{
	TEST_EXPR( points.front().t <= t && t <= points.back().t );

	PointStore::iterator point1, point2;
	for( point2 = points.begin(); 
	    point2 != points.end() && (*point2).t < t; point2++ );

	if( point2 == points.begin() ) {
		point1 = points.begin();
		++point2;
	} else {
		point1 = point2 - 1;
	}
	double length = (*point2).t - (*point1).t;

	t = (t - (*point1).t) / length;

	POINT p0 = (*point1).point;
	POINT p1 = (*point2).point;
	POINT dp0 = (*point1).dpoint;
	POINT dp1 = (*point2).dpoint;
	
	return (1-t*t*(3-2*t)) * p0 + t*t*(3-2*t) * p1 +
		length * t*(t-1)*(t-1) * dp0 + length * t*t*(t-1) * dp1;
}

template< class POINT >
void Hermite<POINT>::update_dpoints()
{
	for( int i = 1; i < points.size()-1; i++ ) {
/*		double	alpha;
		double	beta;

		if( i < points.size()-2 ) {
			alpha = abs( diff(i+1) - diff(i+2) );
		} else {
			alpha = 1;
		}
		if( i > 1 ) {
			beta = abs( diff(i) - diff(i-1) );
		} else {
			beta = 1;
		}
		if( equal(alpha,0) && equal(beta,0) ) {
			alpha = beta = 1/2.;
		}
		if( points[i].point == points[i-1].point || 
		   points[i].point == points[i+1].point ) {
			points[i].dpoint = POINT( 0 );
		} else {
			points[i].dpoint = alpha * diff(i) + beta * diff(i+1);
		}
*/
		points[i].dpoint = (points[i+1].point - points[i-1].point) /
			(points[i+1].t - points[i-1].t);
	}
}

template< class POINT >
inline POINT Hermite<POINT>::diff( int index )
{
	POINT m = (points[index].point - points[index-1].point) /
		(points[index].t - points[index-1].t);
	return m;
}

// ----------------------------------------------------------------------

/*
#include	"viewer.hh"
#include	"vector.hh"

int main()
{
	typedef	Vector< double >	Point;
	typedef	Hermite< Point >	Spline;

	Viewer	viewer;
	Spline::PointStore	points( 3 );

	points[0] = Spline::Parameter( 0, Point( -2, 0, 0 ) );
	points[1] = Spline::Parameter( 30, Point(  0, 0, 2 ) );
	points[2] = Spline::Parameter( 100, Point(  5, 0, 0 ) );
	
	Spline spline( points, Point(0,0,0), Point(0,0,0) );

	viewer.begin();
	viewer.move_to( spline(0) );
 	for( int i = 1; i < 101; i++ ) {
		viewer.line_to( spline(i) );
	}
	viewer.end();

	fgetc(stdin);
	return 0;
}
*/
