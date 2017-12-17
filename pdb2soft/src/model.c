
#include	"model.h"
#include	"matrix.h"
#include	"sphere.h"
#include	"roll.h"

#include	<tinyModel.h>
#include	<tinyMaterial.h>

#define	SQR(x)			((x)*(x))
#define	LENGTH( x, y, z )	(sqrt(SQR(x)+SQR(y)+SQR(z)))
#define	MAX_MATRIX		20

struct _Model {

	DK_Model	*model;
	DK_Model	*spheres;
	DK_Model	*rolls;
	Matrix		*matrix;

	int	horizontal;
	int	vertical;
	int	lo_step;
	int	la_step;
	int	la_base;

};


/*------------------------------------------------------------
 *	private functions
 *------------------------------------------------------------*/

static double Angle( double, double, double, double, double, double );
static DK_Material *GetMaterial( Color *color );

/*----------------------------------------------------------------------
 *static DK_Material *GetMaterial( Color *color );
 *----------------------------------------------------------------------*/

static DK_Material *GetMaterial( Color *color )
{
	DK_Material	*material;

	material = DK_materialAllocate();
	material->ambient.r = color->ambient_r;
	material->ambient.g = color->ambient_g;
	material->ambient.b = color->ambient_b;
	if( color->specular_r >= 0 ) {
		material->specular.r = color->specular_r;
		material->specular.g = color->specular_g;
		material->specular.b = color->specular_b;
	}
	if( color->diffuse_r >= 0 ) {
		material->diffuse.r = color->diffuse_r;
		material->diffuse.g = color->diffuse_g;
		material->diffuse.b = color->diffuse_b;
	} else {
		material->diffuse.r = color->ambient_r * 2./3.;
		material->diffuse.g = color->ambient_g * 2./3.;
		material->diffuse.b = color->ambient_b * 2./3.;
	}

	return material;
}

/*----------------------------------------------------------------------
 *static double Angle( double, double, double, double, double, double );
 *----------------------------------------------------------------------*/

static double Angle( x1, y1, z1, x2, y2, z2 )
	double	x1, y1, z1;
	double	x2, y2, z2;
{
	double	product;
	double	abs1, abs2;
	double	cosinus;

	abs1 = LENGTH( x1, y1, z1 );
	abs2 = LENGTH( x2, y2, z2 );
	if( abs1 == 0 || abs2 == 0 ) return 0;

	product = x1*x2 + y1*y2 + z1*z2;
	cosinus = product / (abs1 * abs2);

	return acos(cosinus);
}

/*------------------------------------------------------------
 *extern Model mdlCreate( horizontal, vertical, lo_step, la_step, la_base );
 *------------------------------------------------------------*/

extern Model mdlCreate( horizontal, vertical, lo_step, la_step, la_base )
	int	horizontal;
	int	vertical;
	int	lo_step;
	int	la_step;
	int	la_base;
{
	Model	m;

	LOG( "mdlCreate" );

	DK_NORMAL = FALSE;

	m = ALLOC( 1, struct _Model );
	m->matrix = mtrxIdentity();
	m->model = DK_modelAllocate();
	m->model->name = strdup( "molecule" );
	m->model->type = DK_MDL_NILL;
	m->spheres = DK_modelAllocate();
	m->spheres->name = strdup( "atoms" );
	m->spheres->type = DK_MDL_NILL;
	m->rolls = DK_modelAllocate();
	m->rolls->name = strdup( "bonds" );
	m->rolls->type = DK_MDL_NILL;
	DK_modelCreateParentship( m->model, m->spheres );
	DK_modelCreateParentship( m->model, m->rolls );
	m->horizontal = horizontal;
	m->vertical = vertical;
	m->lo_step = lo_step;
	m->la_step = la_step;
	m->la_base = la_base;

	return m;
}

/*------------------------------------------------------------
 *extern void mdlSave( Model m, FILE *file );
 *------------------------------------------------------------*/

extern void mdlSave( Model m, FILE *file )
{
	LOG( "mdlSave" );

	DK_fdmodelWriteBinaryFile( file, m->model );
}

/*------------------------------------------------------------
 *extern void mdlFree( Model m );
 *------------------------------------------------------------*/

extern void mdlFree( Model m )
{
	LOG( "mdlFree" );

	DK_modelDispose( &m->model );
	mtrxFree( m->matrix );
	FREE( m );
}

/*------------------------------------------------------------
 *extern void	mdlAddSphere( m, name, x, y, z, r, color );
 *------------------------------------------------------------*/

extern void mdlAddSphere( m, name, x, y, z, r, color )
	Model	m;
	char	*name;
	double	x, y, z;
	double	r;
	Color	*color;
{
	DK_Model	*sphere;
	DK_Material	*material;

	LOG( "mdlAddSphere" );
	
	material = GetMaterial( color );

	mtrxSetIdentity( m->matrix );
	mtrxScale( m->matrix, r, r, r );

	sphere = CreateSphere( name, m->matrix, m->horizontal, m->vertical );
	sphere->translation.x = x;
	sphere->translation.y = y;
	sphere->translation.z = z;
	sphere->materials = material;
	sphere->nbMaterials = 1;

	DK_modelCreateParentship( m->spheres, sphere );
}

/*------------------------------------------------------------
 *extern void mdlAddRoll( m, name, x1, y1, z1, x2, y2, z2, r, color1, color2 );
 *------------------------------------------------------------*/

extern void mdlAddRoll( m, name, x1, y1, z1, x2, y2, z2, r, color1, color2 )
	Model	m;
	char	*name;
	double	x1, y1, z1;
	double	x2, y2, z2;
	double	r;
	Color	*color1, *color2;
{
	double		dx, dy, dz;
	double		ax, ay, az;
	DK_Model	*roll;
	DK_Material	*material1, *material2;
	double		alfa, beta;

	LOG( "mdlAddRoll" );

	dx = x2-x1;
	dy = y2-y1;
	dz = z2-z1;
	mtrxSetIdentity( m->matrix );
	mtrxScale( m->matrix, r, LENGTH(dx, dy, dz), r );
	alfa = Angle( 0, 1, 0, dx, dy, dz );
	beta = Angle( 0, 0, 1, dx, 0, dz );
	if( dx < 0 ) beta *= -1;
	
	mtrxRotate( m->matrix, alfa, 'x' );
	mtrxRotate( m->matrix, beta, 'y' );
	mtrxTransfer( m->matrix, x1, y1, z1 );

	roll = CreateRoll( name, m->matrix, 
			  m->lo_step, m->la_step, m->la_base, 0, 1 );
	material1 = GetMaterial( color1 );
	material2 = GetMaterial( color2 );
	material1->next = material2;
	roll->materials = material1;
	roll->nbMaterials = 2;

	DK_modelCreateParentship( m->rolls, roll );
}

/* main - to test this module

int main()
{
	Model	m;
	Color	color = { 255, 0, 0 };

	OPEN_LOG;
	m = mdlCreate( 4, 8, 8, 2, 2 );
	mdlAddRoll( m, 3, 3, 3, -3, -3, -3, 0.5, NULL );
	mdlAddSphere( m, "atom", 3, 3, 3, 1, &color );
	mdlSave( m, stdout );
	mdlFree( m );
	CLOSE_LOG;

	return 0;
}
*/
