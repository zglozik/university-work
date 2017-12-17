
#include	"stdtypes.h"
#include	<ctype.h>
#include	<stdlib.h>

#ifdef	DEBUG

FILE	*info_file;

#endif

/*------------------------------------------------------------
 *extern void	*Alloc( int size );
 *------------------------------------------------------------*/

extern void *Alloc( int size )
{
	void	*p;

	p = calloc( 1, size );
	if( !p ) {
		fprintf( stderr, "not enough memory, sorry\n" );
		exit( 1 );
	}
	return p;
}

/*------------------------------------------------------------
 *extern char *UpperStr( char *str );
 *------------------------------------------------------------*/

extern char *UpperStr( char *str )
{
	for( ; *str; str++ )
		*str = toupper(*str);

	return str;
}
