
#include	"Person.h"
#include	"sb.h"
#include	"debug.h"
#include	"Assistant.h"
#include	"random.h"

#define	MAX_ORDERS	5
#define	MAX_FOOD	3

Person::Person( const char *name ) : Object( name )
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

	message_queue->in( Message(this, ENTER) );
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
	Person *person = this;
	Timer::setTimerParam param( RandNumber(0,2), 
			   new Message(waiting_queue,
				       WaitingQueue::ENTER_QUEUE,
				       &person, sizeof(person)) );
	message_queue->in( Message(timer, Timer::SET_TIMER,
				   &param, sizeof(param)) );
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
	Person *customer = this;
	message_queue->in( Message(assistant, Assistant::ARE_YOU_READY,
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
		message_queue->in( Message(assistant, Assistant::ENOUGH) );
	}
}

void Person::order()
{
	LOG( "Person::order" );

	int *order = _orders->out();

	Assistant::orderParam param( (Food) (*order & 0xff), *order >> 8 );
	message_queue->in( Message(assistant, Assistant::ORDER,
				   &param, sizeof(param)) );
	delete order;
}

void Person::pay( int money )
{
	LOG( "Person::pay" );

	if( money ) {
		char buf[100];
		sprintf( buf, "Tessek, %d forint.", money );
		print( buf );
		message_queue->in( Message(assistant, Assistant::GIVE_MONEY,
					   &money, sizeof(money)) );
	} else {
		print( "Koszonom." );
		message_queue->in( Message(waiting_queue,
				  WaitingQueue::LEAVE_QUEUE,
				  &_queue_index, sizeof(_queue_index)) );
		Timer::setTimerParam tp( RandNumber(0, 1),
					new Message(this, EXIT) );
		message_queue->in( Message(timer, Timer::SET_TIMER,
					   &tp, sizeof(tp)) );
	}
}

void Person::paid()
{
	LOG( "Person::paid" );

	print( "Koszonom." );
	message_queue->in( Message(waiting_queue, WaitingQueue::LEAVE_QUEUE,
				   &_queue_index, sizeof(_queue_index)) );

	print( "Keresek egy ures helyet a bufeben." );
	Timer::setTimerParam param( RandNumber(1,3), new Message(this,EAT) );
	message_queue->in( Message(timer, Timer::SET_TIMER,
				   &param, sizeof(param)) );
}

void Person::eat()
{
	LOG( "Person::eat" );

	print( "Elkezdtem enni, inni." );
	Timer::setTimerParam param( RandNumber(5, 15),
				   new Message(this, EXIT) );
	message_queue->in( Message(timer, Timer::SET_TIMER,
				   &param, sizeof(param)) );
}

void Person::exit()
{
	LOG( "Person::exit" );

	print( "Kilepek a bufebol." );

	Person	*person = this;
	message_queue->in( Message(manager, Manager::DIE_PERSON,
				   &person, sizeof(person)) );
}
	
