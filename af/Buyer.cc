
#include	"Buyer.h"
#include	"Animals.h"
#include	"Owner.h"
#include	"af.h"
#include	"random.h"
#include	<stdio.h>

Buyer::Buyer() : Object( "vasarlo" )
{
	message_queue.in( Message(this, BUY), rand_number(1,5) );
}

void Buyer::handle_message( const Message &message )
{
	switch( message.message_id() ) {
	    case BUY:
		buy();
		break;
	}
}

void Buyer::buy()
{
	Animal::AnimalType whom = (Animal::AnimalType) 
		rand_number( 0, NUM_ANIMALS-1 );

	char buf[100];
	sprintf( buf, "Akarok vasarolni %s-t.", animal_names[whom] );
	print( buf );
	message_queue.in( Message(&owner, Owner::ASK_FOR_ANIMAL,
				  &whom, sizeof(whom)), 0 );
	message_queue.in( Message(this, BUY), rand_number(5,10) );
}
