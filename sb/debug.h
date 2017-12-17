
#ifndef	DEBUG_H
#define	DEBUG_H


#ifdef	DEBUG

#include	<stdio.h>
#include	<assert.h>

#define	LOG( text )	fprintf( stderr, "function invoked: %s()"	\
				" in file: %s(%d)\n",			\
				text, __FILE__, __LINE__ )

#define	TEST_EXPR( expr )	assert( expr )

#else

#define	LOG( text )
#define	TEST_EXPR( expr )

#endif /* debug */

#endif /* DEBUG_H */
