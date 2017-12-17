
//
//	common member functions for Approximation class
//

#include	"approximation.hh"
#include	"householder.hh"

//----------------------------------------------------------------------
//	init_parameters( kv, parameters )
//----------------------------------------------------------------------

void Approximation::init_parameters( const KnotVector &kv,
				     const Array<double> &parameters )
{
	this->kv = kv;
	this->parameters = parameters;
}

//----------------------------------------------------------------------
//	fill_controls( Array<Point> &controls, Matrix<double> &x,
//		  uint index ) const
//----------------------------------------------------------------------

void Approximation::fill_controls( Array<Point> &controls, Matrix<double> &x,
				  uint index ) const
{
	for( uint i = 0; i < controls.size(); i++ ) {
		controls[i][index] = x(i);
	}
}

//----------------------------------------------------------------------
//	double Approximation::count_diff( const BSplineCurve &curve ) const;
//----------------------------------------------------------------------

double Approximation::count_diff( const BSplineCurve &curve ) const
{
	double	diff = 0;
	Point	d;

	for( uint i = 0; i < measured.size(); i++ ) {
		d = curve( parameters[i] ) - measured[i];
		diff += d * d;
	}
	uint num_points = measured.size();
	return diff / num_points;
}

//----------------------------------------------------------------------
//	solve_problem( const Matrix<double> &A, Matrix<double> &x ) const
//----------------------------------------------------------------------

bool Approximation::solve_problem( const Matrix<double> &A, 
				  Matrix<double> &x ) const
{
	HouseHolder	hh;
	Matrix<double>	B = A;

	hh.solve( B, x );
	return true;
}
