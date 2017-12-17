
#include	"Buyer.h"
#include	"random.h"
#include	<stdio.h>
#include	<unistd.h>

Buyer::Buyer( char *name, int tid ) : Object( name, tid )
{
	message_queue.in( Message(id(), BUY) );
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
	AnimalType whom = (AnimalType) rand_number( 0, NUM_ANIMALS-1 );

	char buf[100];
	sprintf( buf, "Akarok vasarolni %s-t.", animal_names[whom] );
	print( buf );
	message_queue.in(Message(owner, ASK_FOR_ANIMAL, &whom, sizeof(whom)));
	sleep( rand_number(1,3) );
	message_queue.in(Message(id(), BUY));
}

int main()
{
	MainLoop<Buyer> a;

	a();
}
