
#ifndef	_APPROXIMATION_HH_
#define	_APPROXIMATION_HH_

#include	"bspline.hh"
#include	"matrix.hh"

class Approximation {

    public:
	Approximation( const Array<Point> &measured ) : measured(measured) { }

	void init_parameters( const KnotVector &kv,
			      const Array<double> &parameters );
	double approximate1( uint n, BSplineCurve &curve );

    private:
	// common member functions

	bool solve_problem( const Matrix<double> &A, Matrix<double> &x ) const;
	double count_diff( const BSplineCurve &curve ) const;
	void fill_controls( Array<Point> &controls, Matrix<double> &x,
			uint index ) const;

	// member functions for version 1

	void fill_matrix1( Matrix<double> &A, const Base &base ) const;
	void fill_b1( Matrix<double> &A, uint index ) const;

	const Array<Point>	&measured;
	Array<double>		parameters;
	KnotVector		kv;
};

#endif // _APPROXIMATION_HH_
