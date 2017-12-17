
#include	"Object.h"

#include	<stdio.h>

void Object::print( const char *string ) const
{
	printf( "%s: %s\n", name(), string );
	fflush( stdout );
}
