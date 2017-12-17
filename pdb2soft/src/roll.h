
#ifndef	ROLL_H
#define	ROLL_H

#include	"matrix.h"
#include	<tinyModel.h>

extern DK_Model *CreateRoll( char *name, Matrix *, 
			    int lo_step, int la_step, int la_base,
			    int mat1, int mat2 );

#endif /* ROLL_H */
