
#include	"roll.h"

#include	<tinySoftType.h>
#include	<tinyMesh.h>
#include	<tinyModel.h>
#include	<tinyMaterial.h>
#include	<math.h>

struct _Model {

	DK_Model	*rolls;
	Matrix		*matrix;
};


#define SQR(x)                  ((x)*(x))
#define LENGTH( x, y, z )       (sqrt(SQR(x)+SQR(y)+SQR(z)))

/*----------------------------------------------------------------------
 *	private function prototypes
 *----------------------------------------------------------------------*/

static int MakeCircle( DK_Vertex *, int, int, double );
static void MakeVertices( DK_Mesh *, int, int, int, int *, int *, int * );
static void TransformVertices( DK_Mesh *, Matrix * );
static void MakeCylinderTriangles( DK_Polygon *, int, int, int, 
				  DK_Boolean, int );
static void MakeRectangles( DK_Polygon *, int, int, int, int );
static void MakePolygons( DK_Mesh *, int, int, int, int, int, int, int );

static double Angle( double, double, double, double, double, double );
static DK_Material *GetMaterial( Color *color );

/*----------------------------------------------------------------------
 *static int MakeCircle( vertices, lo_step, la_base, y );
 *----------------------------------------------------------------------*/

static int MakeCircle( vertices, lo_step, la_base, y )
	DK_Vertex	*vertices;
	int		lo_step;
	int		la_base;
	double		y;
{
	int	r, i;
	int	points = 0;
	int	base;

	if( la_base > 0 ) {
		_DK_VectorInit( vertices[0].position, 0, y, 0 );
		points++;
		base = 1;
	} else {
		la_base = 1;
		base = 0;
	}

	for( r = 1; r <= la_base; r++ ) {
		double radius = 1.0 / la_base * r;

		for( i = 0; i < lo_step; i++ ) {
			double alfa = i * 2 * M_PI / lo_step;

			_DK_VectorInit( vertices[base + i].position,
				radius * cos(alfa), y, radius * sin(alfa) );
			points++;
		}
		base += lo_step;
	}

	return points;
}

/*----------------------------------------------------------------------
 * static void MakeVertices( mesh, lo_step, la_step, la_base,
 *	top_cylinder, bottom_cylinder, middle_cylinders )
 *----------------------------------------------------------------------*/

static void MakeVertices( mesh, lo_step, la_step, la_base,
			top_cylinder, bottom_cylinder, middle_cylinders )
	DK_Mesh	*mesh;
	int	lo_step;
	int	la_step;
	int	la_base;
	int	*top_cylinder;
	int	*bottom_cylinder;
	int	*middle_cylinders;
{
	int	i;
	int	base;

	mesh->nbVertices = 2 * (lo_step * la_base + 1) + lo_step * (la_step-1);
	mesh->vertices = DK_vertexAllocate( mesh->nbVertices );

	*top_cylinder = 0;
	*bottom_cylinder = lo_step * la_base + 1;
	*middle_cylinders = *bottom_cylinder + lo_step * la_base + 1;

	MakeCircle( &mesh->vertices[*top_cylinder], lo_step, la_base, 1 );
	MakeCircle( &mesh->vertices[*bottom_cylinder], lo_step, la_base, 0 );

	base = *middle_cylinders;
	for( i = 0; i < la_step-1; i++ ) {
		base += MakeCircle( &mesh->vertices[base], lo_step, 0,
			(double) (i+1) / la_step );
	}
}

/*----------------------------------------------------------------------
 *static void MakeCylinderTriangles( polygons, lo_step, la_base, 
 *	cylinder, top, mat );
 *----------------------------------------------------------------------*/

static void MakeCylinderTriangles( polygons, lo_step, la_base, cylinder,
				  top, mat )
	DK_Polygon	*polygons;
	int		lo_step, la_base;
	int		cylinder;
	DK_Boolean	top;
	int		mat;
{
	int	i, j;

	for( i = 0; i < la_base; i++ ) {
		for( j = 0; j < lo_step; j++ ) {
			int start = 1 + i*lo_step + j;
			int end = j == lo_step-1 ? 1 + i*lo_step :
				2 + i*lo_step + j;	

			if( top ) {
				int tmp = start;
				start = cylinder + end;
				end = tmp + cylinder;
			} else {
				start += cylinder;
				end += cylinder;
			}
			DK_polygonInitializeVertex( polygons, 3, 
				start, end, cylinder );
			polygons->material = mat;
			polygons++;
		}
	}
}

/*----------------------------------------------------------------------
 *static void MakeRectangles( polygons, lo_step, src, dst, mat );
 *----------------------------------------------------------------------*/

static void MakeRectangles( polygons, lo_step, src, dst, mat )
	DK_Polygon	*polygons;
	int		lo_step;
	int		src, dst;
	int		mat;
{
	int	i;

	for( i = 0; i < lo_step; i++ ) {
		int	x1, x2, x3, x4;

		x1 = src + i;
		x4 = dst + i;
		if( i == lo_step-1 ) {
			x2 = src;
			x3 = dst;
		} else {
			x2 = src + i + 1;
			x3 = dst + i + 1;
		}
		DK_polygonInitializeVertex( polygons, 4, x2, x1, x4, x3 );
		polygons->material = mat;
		polygons++;
	}
}

/*----------------------------------------------------------------------
 *static void MakePolygons( mesh, lo_step, la_step, la_base,
 *	top_cylinder, bottom_cylinder, middle_cylinders, mat );
 *----------------------------------------------------------------------*/

static void MakePolygons( mesh, lo_step, la_step, la_base,
			top_cylinder, bottom_cylinder, middle_cylinders,
			mat )
	DK_Mesh		*mesh;
	int		lo_step, la_step, la_base;
	int		top_cylinder, bottom_cylinder, middle_cylinders;
	int		mat;
{
	int	base;
	int	source, destination;
	int	i;

	mesh->nbPolygons = (2*la_base + la_step) * lo_step;
	mesh->polygons = DK_polygonAllocate( mesh->nbPolygons );

	MakeCylinderTriangles( &mesh->polygons[0], lo_step, la_base,
		top_cylinder, TRUE, mat );
	MakeCylinderTriangles( &mesh->polygons[lo_step*la_base],
		lo_step, la_base, bottom_cylinder, FALSE, mat );

	base = 2 * lo_step * la_base;
	source = bottom_cylinder + 1 + lo_step*(la_base-1);
	for( i = 0; i < la_step; i++ ) {
		if( i == la_step-1 ) {
			destination = top_cylinder + 1 + lo_step*(la_base-1);
		} else {
			destination = middle_cylinders + lo_step*i;
		}
		MakeRectangles( &mesh->polygons[base], lo_step,
			source, destination, mat );
		base += lo_step;
		source = destination;
	}
}

/*----------------------------------------------------------------------
 *static void TransformVertices( DK_Mesh *mesh, Matrix *matrix );
 *----------------------------------------------------------------------*/

static void TransformVertices( DK_Mesh *mesh, Matrix *matrix )
{
	int	i;

	for( i = 0; i < mesh->nbVertices; i++ ) {
		mtrxMulVector( &mesh->vertices[i].position, matrix );
	}
}

/*----------------------------------------------------------------------
 *extern DK_Model *CreateRoll( name, matrix, lo_step, la_step, la_base, 
 *			      mat );
 *----------------------------------------------------------------------*/

extern DK_Model *CreateRoll( name, matrix, lo_step, la_step, la_base, mat )
	char	*name;
	Matrix	*matrix;
	int	lo_step;
	int	la_step;
	int	la_base;
	int	mat;
{
	DK_Model	*model;
	DK_Mesh		*mesh;
	int		top_cylinder;
	int		bottom_cylinder;
	int		middle_cylinders;

	mesh = DK_meshAllocate();
	MakeVertices( mesh, lo_step, la_step, la_base, 
		&top_cylinder, &bottom_cylinder, &middle_cylinders );
	TransformVertices( mesh, matrix );
	MakePolygons( mesh, lo_step, la_step, la_base, 
		top_cylinder, bottom_cylinder, middle_cylinders, mat );

	model = DK_modelAllocate();
	model->name = strdup( name );
	model->type = DK_MDL_MESH;
	model->definition = mesh;

	return model;
}

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
 *extern Model mdlCreate()
 *------------------------------------------------------------*/

extern Model mdlCreate()
{
	Model	m;

	DK_NORMAL = FALSE;

	m = calloc( 1, sizeof(struct _Model) );
	m->matrix = mtrxIdentity();
	m->rolls = DK_modelAllocate();
	m->rolls->name = strdup( "rolls" );
	m->rolls->type = DK_MDL_NILL;

	return m;
}

/*------------------------------------------------------------
 *extern int mdlSave( Model m, FILE *file );
 *------------------------------------------------------------*/

extern int mdlSave( Model m, FILE *file )
{
	return DK_fdmodelWriteBinaryFile( file, m->rolls );
}

/*------------------------------------------------------------
 *extern void mdlFree( Model m );
 *------------------------------------------------------------*/

extern void mdlFree( Model m )
{
	DK_modelDispose( &m->rolls );
	mtrxFree( m->matrix );
	free( m );
}

/*------------------------------------------------------------
 *extern void mdlAddRoll( m, name, x1, y1, z1, x2, y2, z2, r, color1 );
 *------------------------------------------------------------*/

extern void mdlAddRoll( m, name, x1, y1, z1, x2, y2, z2, r, color1 )
	Model	m;
	char	*name;
	double	x1, y1, z1;
	double	x2, y2, z2;
	double	r;
	Color	*color1;
{
	double		dx, dy, dz;
	double		ax, ay, az;
	DK_Model	*roll;
	DK_Material	*material1;
	double		alfa, beta;

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

	roll = CreateRoll( name, m->matrix, 8, 1, 1, 0 );
	material1 = GetMaterial( color1 );
	roll->materials = material1;
	roll->nbMaterials = 1;

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

