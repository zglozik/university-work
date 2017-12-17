
#ifndef	EQUERRY_H
#define	EQUERRY_H

#include	"Object.h"
#include	"Animals.h"
#include	"generic.h"

class Equerry : public Object {

    public:
	Equerry( const char *name, int tid );

	void handle_message( const Message &message );

    private:

	void add_animal( AnimalData &animal );
	void get_animal( AnimalType whom );
	void feed( AnimalType whom );
	void count_hungry_animals();
	void hungry_answer( bool answer, AnimalType sender );

    private:
	List< AnimalData >	animals;
	int			hungry_animals[NUM_ANIMALS];
	int			all_animals[NUM_ANIMALS];
	int			num_animals;
	bool			wait_for_answers;
	List< AnimalType >	waiting_animals;
	
};

#endif /* EQUERRY_H */
