

#include	"random.h"
#include	<stdlib.h>

extern int rand_number( int low, int high )
{
	return (rand() % (high-low+1)) + low;
}
