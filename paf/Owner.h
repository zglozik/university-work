
#ifndef	OWNER_H
#define	OWNER_H

#include	"Object.h"
#include	"Animals.h"
#include	"Buyer.h"

class Owner : public Object {

    public:
	Owner( const char *name, int tid );

	void handle_message( const Message &message );

    private:

	void give_animal( int animal );
	void create_animal();	
	void ask_for_animal( AnimalType whom );
	void must_feed( AnimalType whom );

    private:
	int	num_animals[NUM_ANIMALS];

};

#endif /* OWNER_H */
