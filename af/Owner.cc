
#include	"Owner.h"
#include	"af.h"
#include	"random.h"
#include	<stdio.h>

Owner::Owner() : Object( "tulajdonos" )
{

	for( int i=0; i < NUM_ANIMALS; i++ ) {
		num_animals[i] = 0;
	}
	message_queue.in( Message(this, CREATE_ANIMAL), rand_number(2, 5) );
}

void Owner::handle_message( const Message &message )
{
	switch( message.message_id() ) {
	    case GIVE_ANIMAL:
		give_animal( *(Animal **) message.data() );
		break;
	    case CREATE_ANIMAL:
		create_animal();
		break;
	    case ASK_FOR_ANIMAL:
		ask_for_animal( *(Animal::AnimalType *) message.data() );
		break;
	    case MUST_FEED:
		must_feed( *(Animal::AnimalType *) message.data() );
		break;
	}
}

void Owner::give_animal( Animal *animal )
{
	if( animal ) {
		char buf[100];
		sprintf( buf, "Eladtam a %s nevu allatot.", animal->name() );
		print( buf );
		delete animal;
	} else {
		print( "Sajnos nincs az istalloban az a fajta allat." );
	}
}

void Owner::create_animal()
{
	int	type = rand_number(0, NUM_ANIMALS-1);
	Animal	*animal;
	char	buf[100];

	num_animals[ type ]++;
	switch( type ) {
	    case Animal::CHICKEN:
		sprintf( buf, "csirke_%02d", num_animals[Animal::CHICKEN] );
		animal = new Chicken( buf );
		break;
	    case Animal::PIG:
		sprintf( buf, "diszno_%02d", num_animals[Animal::PIG] );
		animal = new Pig( buf );
		break;
	}

	message_queue.in( Message(&equerry, Equerry::ADD_ANIMAL,
				  &animal, sizeof(animal)), 0 );
	message_queue.in( Message(this, CREATE_ANIMAL), rand_number(2, 5) );
}
	
void Owner::ask_for_animal( Animal::AnimalType whom )
{
	message_queue.in( Message(&equerry, Equerry::GET_ANIMAL,
				  &whom, sizeof(whom)), 0 );
}

void Owner::must_feed( Animal::AnimalType whom )
{
	char buf[100];
	sprintf( buf, "Tul sok %s ehes, meg kell oket etetni.",
		animal_names[whom] );
	print( buf );
	message_queue.in( Message(&equerry, Equerry::FEED,
				  &whom, sizeof(whom)), 0 );
}
