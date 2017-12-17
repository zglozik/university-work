
#ifndef	PIG_H
#define	PIG_H

#include	"Object.h"
#include	"Animals.h"

class Pig : public Animal {

    public:
	Pig( const char *name, int tid ) : Animal( name, tid )
	{
		animal_type = PIG;
	}

	void eat();

};

#endif /* PIG_H */
