
#ifndef	ANIMALS_H
#define	ANIMALS_H

#include	"Object.h"
#include	"random.h"

#define	NUM_ANIMALS	2

class Equerry;

class Animal : public Object {

    public:
	enum Messages { EAT, IS_HUNGRY };
	enum AnimalType { CHICKEN, PIG };

    protected:
	Animal( const char *name ) : Object( name ), num_questions(0)
	{
		lunch_time = rand_number( 3, 10 );
	}

    public:
	void handle_message( const Message &message );
	AnimalType type() { return animal_type; }

    protected:
	virtual void eat();
	void is_hungry( Equerry *sender );

    protected:

	AnimalType	animal_type;
	int		lunch_time;
	int		num_questions;

};

class Chicken : public Animal {

    public:
	Chicken( const char *name ) : Animal(name) 
	{ 
		animal_type = CHICKEN;
	}

	void eat();

};

class Pig : public Animal {

    public:
	Pig( const char *name ) : Animal(name)
	{
		animal_type = PIG;
	}

	void eat();

};

extern char	*animal_names[];

#endif /* ANIMALS_H */
