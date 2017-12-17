
#include	<errno.h>
#include	<string.h>
#include	<stdarg.h>
#include	"pattern.h"
#include	"finder.h"

/*----------------------------------------------------------------------
 *	private variables
 *----------------------------------------------------------------------*/

const char	*prog_name;
static const char	*regex_str;

/*----------------------------------------------------------------------
 *	void error( const char *str, ... )
 *----------------------------------------------------------------------*/

void error( const char *str, ... )
{
	va_list	args;

	va_start( args, str );
	fprintf( stderr, "%s: ", prog_name );
	vfprintf( stderr, str, args );
	fputc( '\n', stderr );
	va_end( args );
}

/*----------------------------------------------------------------------
 *	FILE *open_file( const char *name, const char *mode )
 *----------------------------------------------------------------------*/

FILE *open_file( const char *name, const char *mode )
{
	FILE	*file;
	if( !(file = fopen(name, mode)) ) {
		error( "%s: %s", name, strerror(errno) );
	}
	return file;
}

/*----------------------------------------------------------------------
 *	bool process_file( FILE *input, Pattern pattern )
 *----------------------------------------------------------------------*/

static bool process_file( FILE *input, Pattern pattern )
{
	char	buffer[1000];
	int	line = 0;
	bool	found = false;

	while( !found && fgets(buffer, 1000, input) != NULL ) {
		++line;
		found = pattern_match( pattern, buffer );
	}
	return found;
}

/*----------------------------------------------------------------------
 *	int parse_options( int argc, char *argv[] )
 *----------------------------------------------------------------------*/

static int parse_options( int argc, char *argv[] )
{
	prog_name = strdup( argv[0] );
	if( argc >= 3 ) {
		regex_str = strdup( argv[1] );
		return 2;
	}
	return 0;
}

/*----------------------------------------------------------------------
 *	void found_file( char *file )
 *----------------------------------------------------------------------*/

static void found_file( char *file )
{
	printf( "%s\n", file );
}

/*----------------------------------------------------------------------
 *	int main( int argc, char *argv[] )
 *----------------------------------------------------------------------*/

int main( int argc, char *argv[] )
{
	int	begin_files;
	FILE	*input;
	Pattern	pattern;

	if( !(begin_files = parse_options(argc, argv)) ) {
		fprintf( stderr, "usage: finder regexp files...\n" );
		exit( 1 );
	}

	pattern = pattern_create( regex_str );
	if( pattern ) {
		for( ; argv[begin_files]; ++begin_files ) {
			input = open_file( argv[begin_files], "r" );
			if( input ) {
				if( process_file(input, pattern) )
					found_file( argv[begin_files] );
				fclose( input );
			}
		}
		pattern_free( pattern );
	}
			
	return 0;
}

