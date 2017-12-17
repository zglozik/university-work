
#ifndef	ANIMALS_H
#define	ANIMALS_H

#include	"Object.h"
#include	"random.h"

class Animal : public Object {

    protected:
	Animal( const char *name, int tid ) :
		Object( name, tid ), num_questions(0), lunch_time(2)
	{ }

    public:
	void handle_message( const Message &message );
	AnimalType type() { return animal_type; }

    protected:
	virtual void eat();
	void is_hungry( int sender );

    protected:

	AnimalType	animal_type;
	int		num_questions;
	int		lunch_time;
};

#endif /* ANIMALS_H */
