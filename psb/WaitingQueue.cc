
#include	"WaitingQueue.h"
#include	"global.h"
#include	<stdio.h>

WaitingQueue::WaitingQueue( const char *name, int tid, int num_queues )
: Object( name, tid )
{
	LOG( "WaitingQueue::WaitingQueue" );

	_num_queues = num_queues;
	_persons = new Queue< int >[ _num_queues ];
}

WaitingQueue::~WaitingQueue()
{
	delete _persons;
}

void WaitingQueue::handleMessage( const Message &message )
{
	LOG( "WaitingQueue::handleMessage" );

	switch( message.messageId() ) {
	    case ENTER_QUEUE:
		enterQueue( *(int *) message.data() );
		break;
	    case LEAVE_QUEUE:
		leaveQueue( *(int *) message.data() );
		break;
	}
}

void WaitingQueue::enterQueue( int person )
{
	LOG( "WaitingQueue::enterQueue" );

	int min = 0;
	for( int i=1; i<_num_queues; i++ ) {
		if( _persons[i].numElements() < _persons[min].numElements() ) {
			min = i;
		}
	}
	_persons[min].in( person );
	message_queue.in( Message(person, WAIT, &min, sizeof(min)) );
	if( _persons[min].numElements() == 1 ) {
		message_queue.in( Message(person, FIRST_IN_QUEUE) );
	}
}

void WaitingQueue::leaveQueue( int index )
{
	LOG( "WaitingQueue::leaveQueue" );

	delete _persons[index].out();
	if( _persons[index].numElements() > 0 ) {
		message_queue.in( Message(*_persons[index].head(),
					  FIRST_IN_QUEUE) );
	}
}

int main()
{
	MainLoop<WaitingQueue> a;

	a();
}
