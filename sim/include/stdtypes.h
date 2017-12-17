
#ifndef	STDTYPES_H
#define	STDTYPES_H

#include	<stdio.h>

#ifdef	DEBUG

#include	<assert.h>

#define	TEST_EXPR( x )	assert( x )
#define	OPEN_LOG	assert( info_file = fopen( "info", "w" ) )
#define	LOG( x )	(fprintf( info_file, "%s\n", x ), fflush(info_file))
#define	INFO( x )	fprintf x;
#define	CLOSE_LOG	fclose( info_file )

extern FILE	*info_file;

#else

#define	TEST_EXPR( x )	;
#define	OPEN_LOG	;
#define	LOG( x )	;
#define	INFO( x )	;
#define	CLOSE_LOG	;

#endif

typedef enum { false, true }	bool;

#define	ALLOC( size, type )	((type *) Alloc( size*sizeof(type)) )
#define	FREE( p )		(free(p), p = NULL)

#define	ARRAY_SIZE( a )		(sizeof(a)/sizeof(a[0]))

extern void	*Alloc( int size );
extern char	*UpperStr( char *str );

#endif /* STDTYPES_H */
