
//
//	member functions for approximation version 1
//

#include	"approximation.hh"
#include	"householder.hh"

//----------------------------------------------------------------------
//	approximate1( n, curve )
//----------------------------------------------------------------------

double Approximation::approximate1( uint un, BSplineCurve &curve )
{
	TEST_EXPR( measured.size() == parameters.size() );

	curve.set_param( un, kv );
	
	uint n = measured.size();
	uint m = kv.size()-un-1;

	Matrix<double> A( n, m+1 );
	Matrix<double> x(m);
	Array<Point> controls(m);

	fill_matrix1( A, curve.ubase() );
	for( uint index = 0; index < 2; index++ ) {
		fill_b1( A, index );
		solve_problem( A, x );
		fill_controls( controls, x, index );
	}
	curve.set_points( controls );

	return count_diff( curve );
}

//----------------------------------------------------------------------
//	fill_matrix1( Matrix<double> &A, const Base &ubase ) const
//----------------------------------------------------------------------

void Approximation::fill_matrix1( Matrix<double> &A, const Base &ubase ) const
{
	uint n = measured.size();
	uint m = ubase.K();

	for( uint i = 0; i < n; i++ ) {
		double p = parameters[i];
		for( uint j = 0; j < m; j++ ) {
			A(i,j) = ubase(j,p);
		}
	}
}

//----------------------------------------------------------------------
//	fill_b1( Matrix<double> &A, uint index ) const
//----------------------------------------------------------------------

void Approximation::fill_b1( Matrix<double> &A, uint index ) const
{
	uint l = A.m()-1;

	for( uint i = 0; i < measured.size(); i++ ) {
		A(i,l) = measured[i][index];
	}
}
