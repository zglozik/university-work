
#include	"Animals.h"
#include	"Equerry.h"
#include	"af.h"

char *animal_names[] = { "csirke", "diszno" };

void Animal::handle_message( const Message &message )
{
	switch( message.message_id() ) {
	    case EAT:
		eat();
		break;
	    case IS_HUNGRY:
		is_hungry( *(Equerry **) message.data() );
		break;
	}
}

void Animal::eat()
{
	num_questions = 0;
	lunch_time = rand_number( 3, 10 );
}

void Animal::is_hungry( Equerry *sender )
{
	Equerry::HungryAnswerParam	param;

	++num_questions;
	param.answer = num_questions >= lunch_time;
	param.animal = this;
	message_queue.in( Message(sender, Equerry::HUNGRY_ANSWER,
				  &param, sizeof(param)), 0 );
}

void Chicken::eat()
{
	Animal::eat();
	print( "Kaptam enni buzadarat, ezen elragcsalok egy kicsit." );
}

void Pig::eat()
{
	Animal::eat();
	print( "Kaptam enni moslekot, finom." );
}
