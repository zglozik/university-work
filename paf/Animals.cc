
#include	"Animals.h"
#include	"random.h"

void Animal::handle_message( const Message &message )
{
	switch( message.message_id() ) {
	    case EAT:
		eat();
		break;
	    case IS_HUNGRY:
		is_hungry( *(int *) message.data() );
		break;
	}
}

void Animal::eat()
{
	num_questions = 0;
	lunch_time = rand_number( 3, 10 );
}

void Animal::is_hungry( int sender )
{
	HungryAnswerParam	param;

	++num_questions;
	param.answer = num_questions >= lunch_time;
	param.animal = type();
	message_queue.in( Message(sender, HUNGRY_ANSWER,
				  &param, sizeof(param)) );
}
