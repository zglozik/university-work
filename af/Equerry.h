
#ifndef	EQUERRY_H
#define	EQUERRY_H

#include	"Object.h"
#include	"Animals.h"
#include	"generic.h"

class Equerry : public Object {

    public:
	enum Messages {
		ADD_ANIMAL, GET_ANIMAL, FEED, COUNT_HUNGRY_ANIMALS,
		HUNGRY_ANSWER
	};

	struct HungryAnswerParam {
		bool	answer;
		Animal	*animal;
	};

	Equerry();

	void handle_message( const Message &message );

    private:

	void add_animal( Animal *animal );
	void get_animal( Animal::AnimalType whom );
	void feed( Animal::AnimalType whom );
	void count_hungry_animals();
	void hungry_answer( bool answer, Animal *sender );

    private:
	List< Animal * >	animals;
	int			hungry_animals[NUM_ANIMALS];
	int			all_animals[NUM_ANIMALS];
	int			num_animals;
	bool			wait_for_answers;
	List< Animal::AnimalType >	waiting_animals;
	
};

#endif /* EQUERRY_H */
