
#include	"Assistant.h"
#include	"debug.h"
#include	"global.h"
#include	<stdio.h>

Assistant::Assistant( const char *name, int tid ) : Object( name, tid )
{
	LOG( "Assistant::Assistant" );

	for( unsigned i=0; i < ARRAY_SIZE(food_quantity); i++ ) {
		food_quantity[i] = 0;
	}
	_customer = -1;
}

void Assistant::handleMessage( const Message &message )
{
	LOG( "Assistant::handleMessage" );

	switch( message.messageId() ) {
	    case ARE_YOU_READY:
		areYouReady( *(int *) message.data() );
		break;
	    case ORDER:
		order( ((AssistantOrderParam *) message.data())->_what,
		      ((AssistantOrderParam *) message.data())->_number );
		break;
	    case ENOUGH:
		enough();
		break;
	    case GIVE_MONEY:
		giveMoney( *(int *) message.data() );
		break;
	    case ADD_FOOD:
		addFood( ((AssistantAddFoodParam *) message.data())->_what,
			((AssistantAddFoodParam *) message.data())->_number );
		break;
	}
}

void Assistant::areYouReady( int customer )
{
	LOG( "Assistant::areYouReady" );
	TEST_EXPR( _customer!=-1 );

	_customer = customer;
	_bill = 0;
	message_queue.in( Message( _customer, ASSISTANT_READY ) );
	
	char buf[100];
	sprintf( buf, "%s pakol a bufeben.", name() );
	print( buf );
}

void Assistant::order( Food what, int number )
{
	LOG( "Assistant::order" );
	TEST_EXPR( _customer!=-1 );

	char buf[100];
	sprintf( buf, "Rendeltek %d darab %s-t.", number, food_names[what] );
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
	message_queue.in( Message( _customer, ANY_MORE ) );
}

void Assistant::enough()
{
	LOG( "Assistant::enough" );
	TEST_EXPR( _customer!=-1 );

	if( _bill ) {
		char buf[100];
		sprintf( buf, "Rendben, irom a szamlat. %d forint lesz.",
			_bill );
		print( buf );
	} else {
		print( "Sajnalom, hogy semmit nem tudott venni." );
	}
	message_queue.in( Message(_customer, PAY, &_bill, sizeof(_bill)) );
}

void Assistant::giveMoney( int /* money */ )
{
	LOG( "Assistant::giveMoney" );
	TEST_EXPR( _customer!=-1 );

	print( "Rendben." );
	message_queue.in( Message(_customer, PAID) );
	_customer = -1;
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

int main()
{
	MainLoop<Assistant> a;

	a();
}
