
#include	"debug.h"
#include	"Person.h"
#include	"global.h"
#include	"random.h"
#include	<stdio.h>

#define	MAX_ORDERS	5
#define	MAX_FOOD	3

Person::Person( const char *name, int tid ) : Object( name, tid )
{
	LOG( "Person::Person" );
	bool	ordered[MAX_ORDERS];
	int	num_orders;

	for( int i=0; i < MAX_ORDERS; i++ )
		ordered[i] = false;

	num_orders = RandNumber( 1, MAX_ORDERS );
	_orders = new Queue< int >;
	for( int i=0; i < num_orders; i++ ) {
		int	what, quantity;

		quantity = RandNumber( 1, MAX_FOOD );
		what = RandNumber( 0, NUM_FOOD-1 );
		while( ordered[ what ] )
			what = RandNumber( 0, NUM_FOOD-1 );
		_orders->in( what + (quantity << 8) );
		ordered[what] = true;
	}

	message_queue.in( Message(id(), ENTER) );
}

Person::~Person()
{
	LOG( "Person::~Person" );

	delete _orders;
}

void Person::handleMessage( const Message &message )
{
	LOG( "Person::handleMessage" );

	switch( message.messageId() ) {
	    case ENTER:
		enter();
		break;
	    case WAIT:
		wait( *(int *) message.data() );
		break;
	    case FIRST_IN_QUEUE:
		firstInQueue();
		break;
	    case ASSISTANT_READY:
		assistantReady();
		break;
	    case ANY_MORE:
		anyMore();
		break;
	    case PAY:
		pay( *(int *) message.data() );
		break;
	    case PAID:
		paid();
		break;
	    case EAT:
		eat();
		break;
	    case EXIT:
		exit();
		break;
	}
}

void Person::enter()
{
	LOG( "Person::enter" );

	print( "Beleptem. Keresem a sort." );
	int i = id();
	message_queue.in( Message(waiting_queue, ENTER_QUEUE, 
			 &i, sizeof(i)) );
}

void Person::wait( int queue_index )
{
	LOG( "Person::wait" );

	_queue_index = queue_index;
	print( "Varok, hogy elso legyek a sorban." );
}

void Person::firstInQueue()
{
	LOG( "Person::firstInQueue" );

	print( "En kovetkezem. Ha az elado figyelne, rendelhetnek." );
	int customer = id();
	message_queue.in( Message(assistant, ARE_YOU_READY,
				  &customer, sizeof(customer)) );
}

void Person::assistantReady()
{
	LOG( "Person::assistantReady" );

	print( "Rendelhetek." );
	order();
}

void Person::anyMore()
{
	LOG( "Person::anyMore" );

	if( !_orders->isEmpty() ) {
		order();
	} else {
		print( "Nem akarok tobbet rendelni." );
		message_queue.in( Message(assistant, ENOUGH) );
	}
}

void Person::order()
{
	LOG( "Person::order" );

	int *order = _orders->out();

	AssistantOrderParam param( (Food) (*order & 0xff), *order >> 8 );
	message_queue.in( Message(assistant, ORDER, &param, sizeof(param)) );
	delete order;
}

void Person::pay( int money )
{
	LOG( "Person::pay" );

	if( money ) {
		char buf[100];
		sprintf( buf, "Tessek, %d forint.", money );
		print( buf );
		message_queue.in( Message(assistant, GIVE_MONEY,
					  &money, sizeof(money)) );
	} else {
		print( "Koszonom." );
		message_queue.in( Message(waiting_queue, LEAVE_QUEUE,
				  &_queue_index, sizeof(_queue_index)) );
		message_queue.in( Message(id(), EXIT) );
	}
}

void Person::paid()
{
	LOG( "Person::paid" );

	print( "Koszonom." );
	message_queue.in( Message(waiting_queue, LEAVE_QUEUE,
				  &_queue_index, sizeof(_queue_index)) );

	print( "Keresek egy ures helyet a bufeben." );
	message_queue.in( Message(id(), EAT) );
}

void Person::eat()
{
	LOG( "Person::eat" );

	print( "Elkezdtem enni, inni." );
	message_queue.in( Message(id(), EXIT) );
}

void Person::exit()
{
	LOG( "Person::exit" );

	print( "Kilepek a bufebol." );

	pvm_exit();
	::exit(0);
}
	
int main()
{
	MainLoop<Person> a;

	a();
}
