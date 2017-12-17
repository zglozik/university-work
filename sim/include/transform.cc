
#include	"angles.hh"

#include	<ctype.h>

template< class ELEM >
Transform<ELEM>::Transform()
{
	for( int i = 0; i < 4; i++ ) {
		for( int j = 0; j < 4; j++ ) {
			matrix[i][j] = i == j;
		}
	}
}

template< class ELEM >
Vector<ELEM> Transform<ELEM>::transform( const Vector<ELEM> &p ) const
{
	Vector<ELEM>	res;

	for( int i = 0; i < 3; i++ ) {

		res[i] = 0;
		for( int j = 0; j < 3; j++ ) {
			res[i] += matrix[i][j] * p[j];
		}
		res[i] += matrix[i][3];
	}

	return res;
}

template< class ELEM >
Transform<ELEM> &Transform<ELEM>::set( ELEM (*data)[4] )
{
	for( int i = 0; i < 4; i++ ) {
		for( int j = 0; j < 4; j++ ) {
			matrix[i][j] = data[i][j];
		}
	}

	return *this;
}

template< class ELEM >
Transform<ELEM> &Transform<ELEM>::rotate( double degree, char axes )
{
	ELEM rx[4][4] =
	{ { 1, 0, 0, 0 },
	  { 0, 0, 0, 0 },
	  { 0, 0, 0, 0 },
	  { 0, 0, 0, 1 } };
	ELEM ry[4][4] =
	{ { 0, 0, 0, 0 },
	  { 0, 1, 0, 0 },
	  { 0, 0, 0, 0 },
	  { 0, 0, 0, 1 } };
	ELEM rz[4][4] =
	{ { 0, 0, 0, 0 },
	  { 0, 0, 0, 0 },
	  { 0, 0, 1, 0 },
	  { 0, 0, 0, 1 } };

	degree = deg2rad(degree);
	switch( toupper(axes) ) {
	    case 'X':
		rx[1][1] = cos(degree);
		rx[2][1] = sin(degree);
		rx[1][2] = -sin(degree);
		rx[2][2] = cos(degree);
		set( rx );
		break;
	    case 'Y':
		ry[0][0] = cos(degree);
		ry[2][0] = -sin(degree);
		ry[0][2] = sin(degree);
		ry[2][2] = cos(degree);
		set( ry );
		break;
	    case 'Z':
		rz[0][0] = cos(degree);
		rz[1][0] = sin(degree);
		rz[0][1] = -sin(degree);
		rz[1][1] = cos(degree);
		set( rz );
		break;
	    default:
		cerr << "sorry, bad axes\n";
		exit( 1 );
	}

	return *this;
}

template< class ELEM >
Transform<ELEM> &Transform<ELEM>::transfer( const ELEM &x, 
					   const ELEM &y, 
					  const ELEM &z )
{
	ELEM	transfer[4][4] =
	{ { 1, 0, 0, 0 },
	  { 0, 1, 0, 0 },
	  { 0, 0, 1, 0 },
	  { 0, 0, 0, 1 } };

	transfer[0][3] = x;
	transfer[1][3] = y;
	transfer[2][3] = z;
	set( transfer );

	return *this;
}

template< class ELEM >
Transform<ELEM> &Transform<ELEM>::scale( double sx, double sy, double sz )
{
	ELEM	scale[4][4] =
	{ { 0, 0, 0, 0 },
	  { 0, 0, 0, 0 },
	  { 0, 0, 0, 0 },
	  { 0, 0, 0, 1 } };

	scale[0][0] = sx;
	scale[1][1] = sy;
	scale[2][2] = sz;
	set( scale );
	
	return *this;
}

template< class ELEM >
Transform<ELEM> operator * ( const Transform<ELEM> &a, 
			    const Transform<ELEM> &b )
{
	Transform<ELEM>	result;

	for( int i = 0; i < 4; i++ ) {
		for( int j = 0; j < 4; j++ ) {
			result.matrix[i][j] = 0;

			for( int k = 0; k < 4; k++ ) {
				result.matrix[i][j] += 
					a.matrix[i][k] * b.matrix[k][j];
			}
		}
	}	

	return result;
}

template< class ELEM >
ostream &operator << ( ostream &s, const Transform<ELEM> &elem )
{
	s << "matrix:";
	for( int i = 0; i < 4; i++ ) {
		for( int j = 0; j < 4; j++ ) {
			s << '\t' << elem.matrix[i][j];
		}
		s << endl;
	}

	return s;
}

//----------------------------------------------------------------------
/*
int main()
{
	Transform<double>	transform;

	transform = Transform<double>().scale( 2, 2, 2 ) *
		Transform<double>().transfer( 1, 1, 1 ) *
		Transform<double>().rotate( 90, 'x' ) *
		Transform<double>().rotate( -90, 'y' ) *
		Transform<double>().rotate( 90, 'z' );
	cout << transform;
	cout << transform.transform( Vector<double>( 1, 0, 0 ) ) << endl;

	return 0;
}
*/
