
#include	"Assistant.h"
#include	"Person.h"
#include	"Timer.h"
#include	"debug.h"
#include	"random.h"
#include	"sb.h"
#include	<stdio.h>

char	*food_names[] = {
	"hamburger",
	"csirke",
	"pizza",
	"coca cola",
	"sprite",
	"fanta",
	"tonic"
};
		
int	food_prices[] = { 130, 160, 300, 50, 45, 45, 60 };

Assistant::Assistant( const char *name ) : Object( name )
{
	LOG( "Assistant::Assistant" );

	for( unsigned i=0; i < ARRAY_SIZE(food_quantity); i++ ) {
		food_quantity[i] = 0;
	}
	_customer = NULL;
}

void Assistant::handleMessage( const Message &message )
{
	LOG( "Assistant::handleMessage" );

	switch( message.messageId() ) {
	    case ARE_YOU_READY:
		areYouReady( *(Person **) message.data() );
		break;
	    case ORDER:
		order( ((orderParam *) message.data())->_what,
		      ((orderParam *) message.data())->_number );
		break;
	    case ENOUGH:
		enough();
		break;
	    case GIVE_MONEY:
		giveMoney( *(int *) message.data() );
		break;
	    case ADD_FOOD:
		addFood( ((addFoodParam *) message.data())->_what,
			((addFoodParam *) message.data())->_number );
		break;
	}
}

void Assistant::areYouReady( Person *customer )
{
	LOG( "Assistant::areYouReady" );
	TEST_EXPR( !_customer );

	_customer = customer;
	_bill = 0;
	int when = RandNumber( 0, 2 );
	Timer::setTimerParam param( when, new Message(_customer,
				Person::ASSISTANT_READY) );
	message_queue->in( Message( timer, Timer::SET_TIMER,
				   &param, sizeof(param) ) );
	if( when ) {
		char buf[100];
		sprintf( buf, "%s pakol a bufeben %d percig.",
			name(), when );
		print( buf );
	}
}

void Assistant::order( Food what, int number )
{
	LOG( "Assistant::order" );
	TEST_EXPR( _customer );

	char buf[100];
	sprintf( buf, "%s rendelt %d darab %s-t.", _customer->name(),
		number, food_names[what] );
	print( buf );
	if( food_quantity[what] > 0 ) {
		int real_number = number > food_quantity[what] ?
			food_quantity[what] : number;

		_bill += real_number * food_prices[what];
		print( "Van raktaron, tessek." );
		if( real_number < number ) {
			char	buf[100];
			sprintf( buf, "De sajnos csak %d darab.",
				real_number );
			print( buf );
		}
		food_quantity[what] -= real_number;
	} else {
		char buf[100];
		sprintf( buf, "Sajnos a %s elfogyott.", food_names[what] );
		print( buf );
	}
	Timer::setTimerParam param( RandNumber(0,1), new Message(_customer,
							Person::ANY_MORE) );
	message_queue->in( Message( timer, Timer::SET_TIMER,
				   &param, sizeof(param) ) );
}

void Assistant::enough()
{
	LOG( "Assistant::enough" );
	TEST_EXPR( _customer );

	if( _bill ) {
		char buf[100];
		sprintf( buf, "Rendben, irom a szamlat. %d forint lesz.",
			_bill );
		print( buf );
	} else {
		print( "Sajnalom, hogy semmit nem tudott venni." );
	}
	Timer::setTimerParam param( RandNumber(0,1), new Message(_customer,
				Person::PAY, &_bill, sizeof(_bill)) );
	message_queue->in( Message( timer, Timer::SET_TIMER,
				   &param, sizeof(param) ) );
}

void Assistant::giveMoney( int /* money */ )
{
	LOG( "Assistant::giveMoney" );
	TEST_EXPR( _customer );

	print( "Rendben." );
	message_queue->in( Message( _customer, Person::PAID ) );
	_customer = NULL;
	_bill = 0;
}

void Assistant::addFood( Food what, int number )
{
	LOG( "Assistant::addFood" );
	char	buf[100];

	sprintf( buf, "Kaptam arut a raktarbol: %s, %d db.",
		food_names[what], number );
	print( buf );
	food_quantity[what] += number;
}
