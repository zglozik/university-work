
#include	"Equerry.h"
#include	"af.h"
#include	"random.h"
#include	<stdio.h>

#define	BORDER	60

Equerry::Equerry() : Object( "istallo" )
{
	wait_for_answers = false;
	message_queue.in( Message(this, COUNT_HUNGRY_ANIMALS), 1 );
}

void Equerry::handle_message( const Message &message )
{
	switch( message.message_id() ) {
	    case ADD_ANIMAL:
		add_animal( *(Animal **) message.data() );
		break;
	    case GET_ANIMAL:
		get_animal( *(Animal::AnimalType *) message.data() );
		break;
	    case FEED:
		feed( *(Animal::AnimalType *) message.data() );
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

void Equerry::add_animal( Animal *animal )
{
	char buf[100];
	sprintf( buf, "Uj allat erkezett a farmra: %s", animal->name() );
	print( buf );
	animals.append( animal );
}

void Equerry::get_animal( Animal::AnimalType whom )
{
	Animal	*animal = NULL;

	if( wait_for_answers ) {
		waiting_animals.append( whom );
	} else {
		for( int i=0; i < animals.numElements() && !animal; i++ ) {
			if( animals[i]->type() == whom ) {
				animal = animals[i];
				animals.removeIndex( i );
			}
		}
		
		if( animal ) {
			char buf[100];
			sprintf( buf, "Kivittek a farmrol a %s nevu allatot.",
				animal->name() );
			print( buf );
		}
		message_queue.in( Message(&owner, Owner::GIVE_ANIMAL,
					  &animal, sizeof(animal)),
				 rand_number(0,1) );
	}
}

void Equerry::feed( Animal::AnimalType whom )
{
	for( int i=0; i < animals.numElements(); i++ ) {
		if( animals[i]->type() == whom ) {
			char buf[100];
			sprintf( buf, "Etetem a %s nevu allatot.",
				animals[i]->name() );
			message_queue.in( Message(animals[i], Animal::EAT),
					 0 );
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
		message_queue.in( Message(this, COUNT_HUNGRY_ANIMALS), 1 );
	} else {
		for( int i=0; i < num_animals; i++ ) {
			Equerry	*object = this;
			message_queue.in( Message( animals[i],
						  Animal::IS_HUNGRY,
						  &object, sizeof(object)),
					 0 );
		}
	}
	wait_for_answers = num_animals != 0;
}

void Equerry::hungry_answer( bool hungry, Animal *sender )
{
	all_animals[ sender->type() ]++;
	if( hungry )
		hungry_animals[ sender->type() ]++;
	int sum = 0;
	for( int i=0; i < NUM_ANIMALS; i++ )
		sum += all_animals[i];
	if( sum == num_animals ) {
		for( int i=0; i < NUM_ANIMALS; i++ ) {
			if( all_animals[i] && 
			   hungry_animals[i]*100/all_animals[i] > BORDER ) {
				message_queue.in( Message(&owner,
							  Owner::MUST_FEED,
							  &i, sizeof(i)), 0 );
			}
		}
		message_queue.in( Message(this, COUNT_HUNGRY_ANIMALS), 1 );
		wait_for_answers = false;
		while( !waiting_animals.isEmpty() ) {
			message_queue.in( Message(this, GET_ANIMAL,
						  &waiting_animals[0],
						  sizeof(waiting_animals[0])),
					 0 );
			waiting_animals.removeIndex( 0 );
		}
	}
}
