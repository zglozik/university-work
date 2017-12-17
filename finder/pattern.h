
#ifndef	PATTERN_H
#define	PATTERN_H

#include	"stdtypes.h"

typedef struct _Pattern	*Pattern;
	
extern Pattern	pattern_create( const char *regex_str );
extern void	pattern_free( Pattern pattern );

extern bool	pattern_match( Pattern pattern, const char *str );

#endif
