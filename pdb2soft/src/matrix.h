
#ifndef	MATRIX_H
#define	MATRIX_H

#include	<tinySoftType.h>

#define	MTRX_HEIGHT	4
#define	MTRX_WIDTH	4

typedef double	Matrix[MTRX_HEIGHT][MTRX_WIDTH];

/*----------------------------------------------------------------------
 *	Matrix
 *----------------------------------------------------------------------*/

extern Matrix	*mtrxCreate( double values[] );
extern Matrix	*mtrxCopy( Matrix *m );
extern void	mtrxFree( Matrix *m );

extern double	mtrxGetValue( Matrix *m, int row, int col );
extern Matrix	*mtrxSetValue( Matrix *m, int row, int col, double value );
extern Matrix	*mtrxMulRight( Matrix *dst, Matrix *src );
extern Matrix	*mtrxMulLeft( Matrix *src, Matrix *dst );
extern Matrix	*mtrxMulScalar( double value, Matrix *dst );
extern DK_Vector *mtrxMulVector( DK_Vector *vector, Matrix *m );

extern Matrix	*mtrxIdentity( void );
extern Matrix	*mtrxSetIdentity( Matrix *m );
extern Matrix	*mtrxScale( Matrix *m, double x, double y, double z );
extern Matrix	*mtrxTransfer( Matrix *m, double x, double y, double z );
extern Matrix	*mtrxRotate( Matrix *m, double angle, char axes );

#endif /* MATRIX_H */
