
#include	<stdio.h>
#include	<tinyModel.h>
#include	<tinyPatch.h>
#include	<tinySpline.h>
#include	<math.h>

#define	D2R( x )	((x) * M_PI / 180)
#define	RADIUS		5
#define	POINTS		4

int CheckNull( void *p )
{
	if( p == NULL ) {
		fprintf( stderr, "can't allocate memory\n" );
		exit( 1 );
	}
	return 1;
}

int main( int argc, char *argv[] )
{
	int		i, j;
	DK_Vector	*pts;
	DK_Spline	*spline;
	DK_Model	*model;
	DK_SplineKey	*key;
	double		radius;

	radius = 3 * RADIUS / (cos(M_PI/POINTS) + 2);

	spline = DK_splineAllocate();
	CheckNull( spline );
	spline->name = strdup( "spline" );
	spline->type = DK_SPL_BSPLINE;
	spline->close = FALSE;
	spline->nbKeys = POINTS + 3;
	spline->keys = DK_splineKeyAllocate( spline->nbKeys );
	for( i = -1, key = &spline->keys[0]; i <= POINTS+1; i++, key++ ) {
		key->position.x = radius * cos( i * M_PI / POINTS );
		key->position.y = radius * sin( i * M_PI / POINTS );
		key->position.z = 0;
		key->previous = key->position;
		key->next = key->position;
	}

	model = DK_modelAllocate();
	model->name = strdup( "patch" );
	model->type = DK_MDL_SPLN;
	model->definition = spline;

	DK_fdmodelWriteBinaryFile( stdout, model );
	DK_modelDispose( &model );

	return 0;
}
	
