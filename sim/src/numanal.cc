
#include	"numanal.hh"
#include	"funcs.hh"
#include	"transform.hh"

// ----------------------------------------------------------------------
//	find_intersection( const Point &c, const Point &p0,
//			const Point &n, const Point &p )
// ----------------------------------------------------------------------

extern double find_intersection( const Point &c, const Point &p0,
				const Point &n, const Point &p )
{
	Point r = p0 - c;
	Point dir = normal(vector_mul(r, n));
	
	double min_deg = 0;	
	double max_deg;
	if( dir == 0 || alpha(dir, Point(0,-1,0)) < 90 ) {
		max_deg = -90;
	} else {
		max_deg = +90;
	}
	double deg;
	bool found = false;
	Transform<double> transform;

	while( !found ) {
		deg = (min_deg + max_deg) / 2;
		transform = 
			Transform<double>().transfer( c[0], c[1], c[2] )*
			Transform<double>().rotate( deg, 'y' ) *
			Transform<double>().transfer( -c[0], -c[1], -c[2] );
		Point newp = transform.transform( p0 );
		double alpha = n * (newp - p);
		if( equal( alpha, 0 ) ) {
			found = true;
		} else if( less( alpha, 0 ) ) {
			min_deg = deg;
		} else {
			max_deg = deg;
		}
	}

	return deg;
}

// ----------------------------------------------------------------------
//	extern Point find_line_intersection( const Point &n1, const Point &p1,
//					    const Point &n2, const Point &p2 );
// ----------------------------------------------------------------------

extern Point find_line_intersection( const Point &n1, const Point &p1,
				    const Point &n2, const Point &p2 )
{
	TEST_EXPR( p1[1] == 0 && p2[1] == 0 );
	double	a, b, c, d;
	double	ai, bi, ci, di;
	
	a = n1[0]; b = n1[2];
	c = n2[0]; d = n2[2];
	double det = (a*d - b*c);

	TEST_EXPR( det != 0 );
	ai = d / det; bi = -b / det;
	ci = -c /det; di = a / det;

	Point m;
	m[0] = ai * (p1*n1) + bi * (p2*n2);
	m[1] = 0;
	m[2] = ci * (p1*n1) + di * (p2*n2);

	return m;
}

// ----------------------------------------------------------------------
/*
int main()
{
	Point	n1(1,0,1);
	Point	p1(0, 0, 0);
	Point	n2(-1, 0, 1);
	Point	p2(0, 0, -2);

	cout << find_line_intersection( n1, p1, n2, p2 ) << endl;

	return 0;
}

*/
