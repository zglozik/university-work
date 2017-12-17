
#include	"matrix.h"
#include	"stdtypes.h"
#include	<ctype.h>
#include	<stdio.h>
#include	<stdlib.h>

/*------------------------------------------------------------
 *extern Matrix *mtrxCreate( double values[] );
 *------------------------------------------------------------*/

extern Matrix *mtrxCreate( double values[] )
{
	Matrix	*m;

	m = calloc( 1, sizeof(Matrix) );
	if( values ) {
		memcpy( *m, values, sizeof(double) * 16 );
	}

	return m;
}

/*------------------------------------------------------------
 *extern Matrix *mtrxCopy( Matrix *m );
 *------------------------------------------------------------*/

extern Matrix *mtrxCopy( Matrix *m )
{
	return mtrxCreate( (double *) *m );
}

/*------------------------------------------------------------
 *extern void mtrxFree( Matrix *m );
 *------------------------------------------------------------*/

extern void mtrxFree( Matrix *m )
{
	free( m );
}

/*------------------------------------------------------------
 *extern double mtrxGetValue( Matrix *m, int row, int col );
 *------------------------------------------------------------*/

extern double mtrxGetValue( Matrix *m, int row, int col )
{
	return (*m)[row][col];
}

/*------------------------------------------------------------
 *extern Matrix *mtrxSetValue( Matrix *m, int row, int col, double value );
 *------------------------------------------------------------*/

extern Matrix *mtrxSetValue( Matrix *m, int row, int col, double value )
{
	(*m)[row][col] = value;
	return m;
}

/*------------------------------------------------------------
 *extern Matrix *mtrxMulRight( Matrix *dst, Matrix *src );
 *------------------------------------------------------------*/

extern Matrix *mtrxMulRight( Matrix *dst, Matrix *src )
{
	int	i, j, k;
	double	result[MTRX_WIDTH];

	for( i = 0; i < MTRX_HEIGHT; i++ ) {
		for( k = 0; k < MTRX_WIDTH; k++ )
			result[k] = 0;
		for( j = 0; j < MTRX_WIDTH; j++ ) {
			for( k = 0; k < MTRX_WIDTH; k++ ) {
				result[j] += (*dst)[i][k] * (*src)[k][j];
			}
		}
		for( k = 0; k < MTRX_WIDTH; k++ )
			(*dst)[i][k] = result[k];
	}

	return dst;
}

/*------------------------------------------------------------
 *extern Matrix *mtrxMulLeft( Matrix *src, Matrix *dst );
 *------------------------------------------------------------*/

extern Matrix *mtrxMulLeft( Matrix *src, Matrix *dst )
{
	int	i, j, k;
	double	result[MTRX_HEIGHT];

	for( j = 0; j < MTRX_WIDTH; j++ ) {
		for( k = 0; k < MTRX_HEIGHT; k++ )
			result[k] = 0;
		for( i = 0; i < MTRX_HEIGHT; i++ ) {
			for( k = 0; k < MTRX_WIDTH; k++ ) {
				result[i] += (*src)[i][k] * (*dst)[k][j];
			}
		}
		for( k = 0; k < MTRX_HEIGHT; k++ )
			(*dst)[k][j] = result[k];
	}	

	return dst;
}

/*------------------------------------------------------------
 *extern Matrix *mtrxMulScalar( double value, Matrix *dst );
 *------------------------------------------------------------*/

extern Matrix *mtrxMulScalar( double value, Matrix *dst )
{
	int	i, j;

	for( i = 0; i < MTRX_HEIGHT; i++ ) {
		for( j = 0; j < MTRX_WIDTH; j++ ) {
			(*dst)[i][j] *= value;
		}
	}

	return dst;
}

/*----------------------------------------------------------------------
 * extern DK_Vector *mtrxMulVector( DK_Vector *vector, Matrix *m );
 *----------------------------------------------------------------------*/

extern DK_Vector *mtrxMulVector( DK_Vector *vector, Matrix *m )
{
	int	j;

	double	result[4];

	for( j = 0; j < 4; j++ ) {
		result[j] = vector->x * (*m)[0][j] +
			vector->y * (*m)[1][j] +
			vector->z * (*m)[2][j] +
			(*m)[3][j];
	}
	vector->x = result[0] / result[3];
	vector->y = result[1] / result[3];
	vector->z = result[2] / result[3];

	return vector;
}

/*----------------------------------------------------------------------
 *extern Matrix *mtrxIdentity();
 *----------------------------------------------------------------------*/

extern Matrix *mtrxIdentity()
{
	double	v[16] = 
	{ 1, 0, 0, 0,
	  0, 1, 0, 0,
          0, 0, 1, 0,
	  0, 0, 0, 1 };

	return mtrxCreate( v );
}

/*----------------------------------------------------------------------
 *extern Matrix *mtrxSetIdentity( Matrix *m );
 *----------------------------------------------------------------------*/

extern Matrix *mtrxSetIdentity( Matrix *m )
{
	int	i;

	memset( *m, 0, sizeof((*m)[0][0]) * MTRX_WIDTH * MTRX_HEIGHT );
	for( i = 0; i < MTRX_HEIGHT; i++ ) {
		(*m)[i][i] = 1;
	}

	return m;
}

/*----------------------------------------------------------------------
 *extern Matrix *mtrxScale( Matrix *m, double x, double y, double z );
 *----------------------------------------------------------------------*/

extern Matrix *mtrxScale( Matrix *m, double x, double y, double z )
{
	double	scale[16] =
	{ 0, 0, 0, 0,
	  0, 0, 0, 0,
	  0, 0, 0, 0,
	  0, 0, 0, 1 };
	Matrix	*s;
	
	scale[0] = x;
	scale[5] = y;
	scale[10] = z;
	s = mtrxCreate( scale );
	mtrxMulRight( m, s );
	mtrxFree( s );

	return m;
}

/*----------------------------------------------------------------------
 *extern Matrix *mtrxTransfer( Matrix *m, double x, double y, double z );
 *----------------------------------------------------------------------*/

extern Matrix *mtrxTransfer( Matrix *m, double x, double y, double z )
{
	double	transfer[16] =
	{ 1, 0, 0, 0,
	  0, 1, 0, 0,
	  0, 0, 1, 0,
	  0, 0, 0, 1 };
	Matrix	*t;

	transfer[12] = x;
	transfer[13] = y;
	transfer[14] = z;
	t = mtrxCreate( transfer );
	mtrxMulRight( m, t );
	mtrxFree( t );

	return m;
}

/*----------------------------------------------------------------------
 *extern Matrix *mtrxRotate( Matrix *m, double angle, char axes );
 *----------------------------------------------------------------------*/

extern Matrix *mtrxRotate( Matrix *m, double angle, char axes )
{
	double	rx[16] =
	{ 1, 0, 0, 0,
	  0, 0, 0, 0,
	  0, 0, 0, 0,
	  0, 0, 0, 1 };
	double	ry[16] =
	{ 0, 0, 0, 0,
	  0, 1, 0, 0,
	  0, 0, 0, 0,
	  0, 0, 0, 1 };
	double	rz[16] =
	{ 0, 0, 0, 0,
	  0, 0, 0, 0,
	  0, 0, 1, 0,
	  0, 0, 0, 1 };

	Matrix	*r;

	switch( toupper(axes) ) {
	    case 'X':
		rx[5] = cos(angle);
		rx[6] = sin(angle);
		rx[9] = -sin(angle);
		rx[10] = cos(angle);
		r = mtrxCreate( rx );
		break;
	    case 'Y':
		ry[0] = cos(angle);
		ry[2] = -sin(angle);
		ry[8] = sin(angle);
		ry[10] = cos(angle);
		r = mtrxCreate( ry );
		break;
	    case 'Z':
		rz[0] = cos(angle);
		rz[1] = sin(angle);
		rz[4] = -sin(angle);
		rz[5] = cos(angle);
		r = mtrxCreate( rz );
		break;
	    default:
		fprintf( stderr, "sorry, bad axes\n" );
		exit( 1 );
	}
	mtrxMulRight( m, r );
	mtrxFree( r );

	return m;
}


/* test main program */

void print( Matrix *m )
{
	int	i, j;

	for( i = 0; i < MTRX_HEIGHT; i++ ) {
		for( j = 0; j < MTRX_WIDTH; j++ ) {
			printf( "%f ", mtrxGetValue(m, i, j) );
		}
		fputc( '\n', stdout );
	}
}

/*
int main()
{
	double	v1[16] = 
	{ 1, 0, 0, 0,
	  0, 1, 0, 0,
          0, 0, 1, 0,
	  0, 0, 0, 1 };
	double	v2[16] = 
	{ 0, 1, 0, 0,
	  1, 0, 0, 0,
          0, 0, 0, 1,
	  0, 0, 1, 0 };
	Matrix	*m1;
	Matrix	*m2;

	m1 = mtrxCreate( v1 );
	m2 = mtrxIdentity();
	mtrxRotate( m2, M_PI_4, 'z' );
	
	print( m2 );

	return 0;
}
*/
