
#include	"molecule.h"
#include	"model.h"

#include	<tinySoftType.h>
#include	<stdio.h>
#include	<stdlib.h>

#define	HYDROGEN_RAD	1.0	/* radius of hydrogen atom */
#define	SCALE		2.5

static int	vertical = 8;
static int	horizontal = 8;
static int	lo_step = 8;
static int	la_step = 2;
static int	la_base = 1;
static FILE	*input = NULL;
static FILE	*output = NULL;

/*------------------------------------------------------------
 *	Function prototypes
 *------------------------------------------------------------*/

static FILE *OpenFile( char *path, char *mode );
static void Usage( void );
static void BuildModel( Molecule *m, Model model );
static int  ParseParameters( int argc, char *argv[] );


/*------------------------------------------------------------
 *static void Usage();
 *------------------------------------------------------------*/

static void Usage()
{
	fprintf( stderr, 
		"Usage: pdb2soft [-v <vertical>] [-h <horziontal>] "
		"[-b <base>] [-l <longitudinal>] [-r <step>] "
		"[<input_file> [<output_file>]]\n"
		"	<inputfile> and <output_file> can be -, which means"
		" standard input and output\n" );
}

/*------------------------------------------------------------
 *static FILE *OpenFile( char *path, char *mode );
 *------------------------------------------------------------*/

static FILE *OpenFile( char *path, char *mode )
{
	FILE	*file;

	if( !(file = fopen( path, mode )) ) {
		fprintf( stderr, "can't open file: %s\n", path );
		return NULL;
	}

	return file;
}

/*----------------------------------------------------------------------
 *static int ParseParameters( int argc, char *argv[] );
 *----------------------------------------------------------------------*/

static int ParseParameters( int argc, char *argv[] )
{
	extern int	optind;
	extern char	*optarg;
	extern int	opterr;
	int		c;
	int		err = FALSE;

	while( !err && (c = getopt(argc, argv, "v:h:b:l:r:")) != EOF ) {
		switch( c ) {
		    case 'v':
			vertical = atoi( optarg );
			if( vertical < 2 ) {
				fprintf( stderr, "bad argument for -v\n" );
				err = TRUE;
			}
			break;
		    case 'h':
			horizontal = atoi( optarg );
			if( horizontal < 4 ) {
				fprintf( stderr, "bad argument for -h\n" );
				err = TRUE;
			}
			break;
		    case 'b':
			la_base = atoi( optarg );
			if( la_base < 1 ) {
				fprintf( stderr, "bad argument for -b\n" );
				err = TRUE;
			}
			break;
		    case 'l':
			lo_step = atoi( optarg );
			if( lo_step < 4 ) {
				fprintf( stderr, "bad argument for -l\n" );
				err = TRUE;
			}
			break;
		    case 'r':
			la_step = atoi( optarg );
			if( la_step < 1 ) {
				fprintf( stderr, "bad argument for -r\n" );
				err = TRUE;
			}
			break;
		    case '?':
			err = TRUE;
			break;
		}
	}
	if( !err ) {
		input = stdin;
		output = stdout;
		if( optind < argc ) {
			if( strcmp( "-", argv[optind] ) )
				input = OpenFile( argv[optind], "r" );
			optind++;
		}
		if( optind < argc ) {
			if( strcmp( "-", argv[optind] ) )
				output = OpenFile( argv[optind], "w" );
			optind++;
		}
		err = !input || !output || optind != argc;
	}
	
	return err;
}

/*------------------------------------------------------------
 *static void BuildModel( Molecule *m, Model model );
 *------------------------------------------------------------*/

static void BuildModel( Molecule *m, Model model )
{
	Chain	*c;
	Group	*g;
	Atom	*a;
	Bond	*b;
	char	name[100];

	for( c = m->chains; c; c = c->next ) {
		for( g = c->groups; g; g = g->next ) {
			for( a = g->atoms; a; a = a->next ) {
				strcpy( name, GetName(a) );
				mdlAddSphere( model, name,
				     SCALE * a->x, SCALE * a->y, SCALE * a->z, 
				     HYDROGEN_RAD*GetRadius(a), GetColor(a) );
			}
		}
	}
	for( b = m->bonds; b; b = b->next ) {
		if( b->flag & BOND_CONECT ) {
			sprintf( name, "%s -- %s", 
				GetName(b->src), GetName(b->dst) );
			mdlAddRoll( model, name,
			    SCALE*b->src->x, SCALE*b->src->y, SCALE*b->src->z,
			    SCALE*b->dst->x, SCALE*b->dst->y, SCALE*b->dst->z,
			    HYDROGEN_RAD / 2.,
			    GetColor(b->src), GetColor(b->dst) );
		}
	}
}

/*------------------------------------------------------------
 *			M  A  I  N
 *------------------------------------------------------------*/

int main( int argc, char *argv[] )
{
	Molecule	*m;
	Model		model;

	OPEN_LOG;

	if( ParseParameters( argc, argv ) ) {
		Usage();
		exit( 1 );
	}

	m = MlclCreate( input );
	INFO(( info_file, "atoms: %d, bonds: %d\n",
		m->main_atoms_no, m->bonds_no ));
	model = mdlCreate( horizontal, vertical, lo_step, la_step, la_base );
	BuildModel( m, model );
	mdlSave( model, output );
	mdlFree( model );

	MlclFree( m );

	fclose( input );
	fclose( output );
	CLOSE_LOG;

	return 0;
}
