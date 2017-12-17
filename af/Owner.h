
#ifndef	OWNER_H
#define	OWNER_H

#include	"Object.h"
#include	"Animals.h"
#include	"Buyer.h"

class Owner : public Object {

    public:
	enum Messages { GIVE_ANIMAL, CREATE_ANIMAL,
			ASK_FOR_ANIMAL, MUST_FEED };

	Owner();

	void handle_message( const Message &message );

    private:

	void give_animal( Animal *animal );
	void create_animal();	
	void ask_for_animal( Animal::AnimalType whom );
	void must_feed( Animal::AnimalType whom );

    private:
	int	num_animals[NUM_ANIMALS];

};

#endif /* OWNER_H */
