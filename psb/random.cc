

#include	"random.h"
#include	<stdlib.h>

extern int RandNumber( int low, int high )
{
	return (rand() % (high-low+1)) + low;
}
