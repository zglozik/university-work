
#include	"Equerry.h"
#include	<stdio.h>
#include	<unistd.h>

#define	BORDER	60

Equerry::Equerry( const char *name, int tid ) : Object( name, tid )
{
	wait_for_answers = false;
	message_queue.in( Message(id(), COUNT_HUNGRY_ANIMALS) );
}

void Equerry::handle_message( const Message &message )
{
	switch( message.message_id() ) {
	    case ADD_ANIMAL:
		add_animal( *(AnimalData *) message.data() );
		break;
	    case GET_ANIMAL:
		get_animal( *(AnimalType *) message.data() );
		break;
	    case FEED:
		feed( *(AnimalType *) message.data() );
		break;
	    case COUNT_HUNGRY_ANIMALS:
		count_hungry_animals();
		break;
	    case HUNGRY_ANSWER:
		hungry_answer( ((HungryAnswerParam *) message.data())->answer,
			      ((HungryAnswerParam *) message.data())->animal );
		break;
	}
}

void Equerry::add_animal( AnimalData &animal )
{
	print( "Uj allat erkezett a farmra." );
	animals.append( animal );
}

void Equerry::get_animal( AnimalType whom )
{
	int animal = -1;

	if( wait_for_answers ) {
		waiting_animals.append( whom );
	} else {
		int	found = -1;

		for( int i=0; i < animals.numElements() && found < 0; i++ ) {
			if( animals[i].type == whom ) {
				found = i;
				animal = animals[i].animal;
			}
		}
		
		if( found >= 0 ) {
			char	buf[100];
			sprintf( buf, "Kivittek a farmrol egy %s allatot.",
				animal_names[animals[found].type] );
			print( buf );
			animals.removeIndex( found );
		}
		message_queue.in( Message(owner, GIVE_ANIMAL,
					  &animal, sizeof(animal)) );
	}
}

void Equerry::feed( AnimalType whom )
{
	for( int i=0; i < animals.numElements(); i++ ) {
		if( animals[i].type == whom ) {
			char	buf[100];

			sprintf( buf, "Etetek egy %s-t.", 
				animal_names[animals[i].type] );
			print( buf );
			message_queue.in( Message(animals[i].animal, EAT) );
		}
	}
}

void Equerry::count_hungry_animals()
{
	for( int i=0; i < NUM_ANIMALS; i++ ) {
		hungry_animals[i] = 0;
		all_animals[i] = 0;
	}
	num_animals = animals.numElements();
	if( !num_animals ) {
		sleep( 1 );
		message_queue.in( Message(id(), COUNT_HUNGRY_ANIMALS) );
	} else {
		for( int i=0; i < num_animals; i++ ) {
			int object = id();
			message_queue.in( Message( animals[i].animal,
						  IS_HUNGRY,
						  &object, sizeof(object)) );
		}
	}
	wait_for_answers = num_animals != 0;
}

void Equerry::hungry_answer( bool hungry, AnimalType sender )
{
	all_animals[ sender ]++;
	if( hungry )
		hungry_animals[ sender ]++;
	int sum = 0;
	for( int i=0; i < NUM_ANIMALS; i++ )
		sum += all_animals[i];
	if( sum == num_animals ) {
		for( int i=0; i < NUM_ANIMALS; i++ ) {
			if( all_animals[i] && 
			   hungry_animals[i]*100/all_animals[i] > BORDER ) {
				message_queue.in( Message(owner, MUST_FEED,
							  &i, sizeof(i)) );
			}
		}
		if( waiting_animals.isEmpty() ) sleep( 2 );
		while( !waiting_animals.isEmpty() ) {
			message_queue.in( Message(id(), GET_ANIMAL,
						  &waiting_animals[0],
						  sizeof(waiting_animals[0])));
			waiting_animals.removeIndex( 0 );
		}
		message_queue.in( Message(id(), COUNT_HUNGRY_ANIMALS) );
		wait_for_answers = false;
	}
}

int main()
{
	MainLoop<Equerry> a;

	a();
}
