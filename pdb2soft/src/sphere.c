
#include	"sphere.h"

#include	<tinyPatch.h>
#include	<math.h>

#define	USTEP	3
#define	VSTEP	3

#define	RADIUS	1

static DK_Vector *CreatePoints( int, int );

/*----------------------------------------------------------------------
 *static DK_Vector *CreatePoints( horizontal, vertical );
 *----------------------------------------------------------------------*/

static DK_Vector *CreatePoints( horizontal, vertical )
	int	horizontal;
	int	vertical;
{
	DK_Vector	*pts;
	int		i, j;
	double		r0;

	pts = DK_patchAllocatePoints( vertical+3, horizontal);
	r0 = 3.0 * RADIUS / (cos(M_PI / vertical) + 2);

	for( i = -1; i <= vertical+1; i++ ) {
		double beta = i * M_PI / vertical;
		double r1 = r0 * sin( beta ) * 3 / 
			(cos(2*M_PI/horizontal) + 2);
		double y = r0 * cos( beta );

		for( j = 0; j < horizontal; j++ ) {
			double alfa =  j * 2 * M_PI / horizontal;

			pts[(i+1)*horizontal+j].y = y;
			pts[(i+1)*horizontal+j].x = r1 * cos( alfa );
			pts[(i+1)*horizontal+j].z = r1 * sin( alfa );
		}
	}

	return pts;
}

/*----------------------------------------------------------------------
 *extern DK_Model *CreateSphere( name, matrix, horizontal, vertical );
 *----------------------------------------------------------------------*/

extern DK_Model *CreateSphere( name, matrix, horizontal, vertical )
	char	*name;
	Matrix	*matrix;
	int	horizontal;
	int	vertical;
{
	int		i, num_pts;
	DK_Vector	*pts;
	DK_Patch	*patch;
	DK_Model	*model;

	pts = CreatePoints( horizontal, vertical );

	patch = DK_patchAllocate();
	patch->uNbPoints = vertical+3;
	patch->vNbPoints = horizontal;
	num_pts = patch->uNbPoints*patch->vNbPoints;
	for( i = 0; i < num_pts; i++ ) {
		mtrxMulVector( &pts[i], matrix );
	}
	patch->points = pts;
	patch->sel =  (short *) calloc( num_pts, sizeof(short) );
	patch->uType = DK_SPL_BSPLINE;
	patch->vType = DK_SPL_BSPLINE;
	patch->uClose = FALSE;
	patch->vClose = TRUE;
	patch->uStep = USTEP;
	patch->vStep = VSTEP;

	model = DK_modelAllocate();
	model->name = strdup( name );
	model->type = DK_MDL_PTCH;
	model->definition = patch;

	return model;
}
