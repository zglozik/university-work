
#include	"Owner.h"
#include	"random.h"
#include	<pvm3.h>
#include	<stdio.h>
#include	<unistd.h>

Owner::Owner( const char *name, int tid ) : Object( name, tid )
{

	for( int i=0; i < NUM_ANIMALS; i++ ) {
		num_animals[i] = 0;
	}
	message_queue.in( Message(id(), CREATE_ANIMAL) );
}

void Owner::handle_message( const Message &message )
{
	switch( message.message_id() ) {
	    case GIVE_ANIMAL:
		give_animal( *(int *) message.data() );
		break;
	    case CREATE_ANIMAL:
		create_animal();
		break;
	    case ASK_FOR_ANIMAL:
		ask_for_animal( *(AnimalType *) message.data() );
		break;
	    case MUST_FEED:
		must_feed( *(AnimalType *) message.data() );
		break;
	}
}

void Owner::give_animal( int animal )
{
	if( animal >= 0 ) {
		print( "Eladtam az allatot." );
		pvm_kill( animal );
	} else {
		print( "Sajnos nincs az istalloban az a fajta allat." );
	}
}

void Owner::create_animal()
{
	AnimalType	type = (AnimalType) rand_number(0, NUM_ANIMALS-1);
	int		animal;
	char		buf[100];
	char		buf2[100];

	num_animals[ type ]++;
	switch( type ) {
	    case CHICKEN:
		sprintf( buf, "csirke_%02d", num_animals[CHICKEN] );
		animal = NewTask( "Chicken" , buf );
		break;
	    case PIG:
		sprintf( buf, "diszno_%02d", num_animals[PIG] );
		animal = NewTask( "Pig", buf );
		break;
	}
	SendGlobal( animal );
	sprintf( buf2, "A %s nevu allatot kuldom a farmra.", buf );
	print( buf2 );

	AnimalData	data;
	data.animal = animal;
	data.type = type;
	message_queue.in( Message(equerry,ADD_ANIMAL,&data,sizeof(data)));
	sleep( rand_number(1,3) );
	message_queue.in( Message(id(), CREATE_ANIMAL) );
}
	
void Owner::ask_for_animal( AnimalType whom )
{
	message_queue.in( Message(equerry, GET_ANIMAL, &whom, sizeof(whom)) );
}

void Owner::must_feed( AnimalType whom )
{
	char buf[100];
	sprintf( buf, "Tul sok %s ehes, meg kell oket etetni.",
		animal_names[whom] );
	print( buf );
	message_queue.in( Message(equerry, FEED, &whom, sizeof(whom)) );
}

int main()
{
	MainLoop<Owner> a;

	a();
}
