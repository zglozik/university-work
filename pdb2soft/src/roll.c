
#include	"roll.h"

#include	<tinySoftType.h>
#include	<tinyMesh.h>

/*----------------------------------------------------------------------
 *	private function prototypes
 *----------------------------------------------------------------------*/

static int MakeCircle( DK_Vertex *, int, int, double );
static void MakeVertices( DK_Mesh *, int, int, int, int *, int *, int * );
static void TransformVertices( DK_Mesh *, Matrix * );
static void MakeCylinderTriangles( DK_Polygon *, int, int, int, 
				  DK_Boolean, int );
static void MakeRectangles( DK_Polygon *, int, int, int, int );
static void MakePolygons( DK_Mesh *, int, int, int, int, int, int, int, int );

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
 *	top_cylinder, bottom_cylinder, middle_cylinders, mat1, mat2 );
 *----------------------------------------------------------------------*/

static void MakePolygons( mesh, lo_step, la_step, la_base,
			top_cylinder, bottom_cylinder, middle_cylinders,
			mat1, mat2 )
	DK_Mesh		*mesh;
	int		lo_step, la_step, la_base;
	int		top_cylinder, bottom_cylinder, middle_cylinders;
	int		mat1, mat2;
{
	int	base;
	int	source, destination;
	int	i;

	mesh->nbPolygons = (2*la_base + la_step) * lo_step;
	mesh->polygons = DK_polygonAllocate( mesh->nbPolygons );

	MakeCylinderTriangles( &mesh->polygons[0], lo_step, la_base,
		top_cylinder, TRUE, mat2 );
	MakeCylinderTriangles( &mesh->polygons[lo_step*la_base],
		lo_step, la_base, bottom_cylinder, FALSE, mat1 );

	base = 2 * lo_step * la_base;
	source = bottom_cylinder + 1 + lo_step*(la_base-1);
	for( i = 0; i < la_step; i++ ) {
		if( i == la_step-1 ) {
			destination = top_cylinder + 1 + lo_step*(la_base-1);
		} else {
			destination = middle_cylinders + lo_step*i;
		}
		MakeRectangles( &mesh->polygons[base], lo_step,
			source, destination, (i < la_step/2) ? mat1 : mat2 );
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
 *			      mat1, mat2 );
 *----------------------------------------------------------------------*/

extern DK_Model *CreateRoll( name, matrix, lo_step, la_step, la_base,
			    mat1, mat2 )
	char	*name;
	Matrix	*matrix;
	int	lo_step;
	int	la_step;
	int	la_base;
	int	mat1;
	int	mat2;
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
		top_cylinder, bottom_cylinder, middle_cylinders,
		mat1, mat2 );

	model = DK_modelAllocate();
	model->name = strdup( name );
	model->type = DK_MDL_MESH;
	model->definition = mesh;

	return model;
}

/* to test this module

int main()
{
	int	lo_step = 8;
	int	la_step = 3;
	int	la_base = 2;
	int	top_cylinder;
	int	bottom_cylinder;
	int	middle_cylinders;

	DK_Model	*model;
	DK_Mesh		*mesh;

	DK_NORMAL = FALSE;
	mesh = DK_meshAllocate();
	MakeVertices( mesh, lo_step, la_step, la_base, 
		&top_cylinder, &bottom_cylinder, &middle_cylinders );
	MakePolygons( mesh, lo_step, la_step, la_base, 
		top_cylinder, bottom_cylinder, middle_cylinders );

	model = DK_modelAllocate();
	model->name = strdup( "mesh" );
	model->type = DK_MDL_MESH;
	model->definition = mesh;

	DK_fdmodelWriteBinaryFile( stdout, model );
	DK_modelDispose( &model );

	return 0;
}
*/
