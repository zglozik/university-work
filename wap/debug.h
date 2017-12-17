
#ifndef _DEBUG_H_
#define _DEBUG_H_

/*
 * Useful macros for checking pre- and post conditions and
 * printing out debugging information.
 */

#ifndef NDEBUG

#include	<stdio.h>

#define	PRINTF(x)	{ \
	printf("%s(%s:%d): ", __FUNCTION__, __FILE__, __LINE__); \
        printf x; }

#else

#define	PRINTF(x)	;

#endif /* DEBUG */

#endif /* _DEBUG_H_ */
