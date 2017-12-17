
#ifndef	CHICKEN_H
#define	CHICKEN_H

#include	"Object.h"
#include	"Animals.h"

class Chicken : public Animal {

    public:
	Chicken( const char *name, int tid ) : Animal(name, tid) 
	{ 
		animal_type = CHICKEN;
	}

	void eat();

};

#endif /* CHICKEN_H */
