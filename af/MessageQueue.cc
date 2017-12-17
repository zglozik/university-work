
#include	"MessageQueue.h"


MessageQueue::MessageQueue()
{
	last_time = 0;
}

void MessageQueue::in( const Message &message, int when )
{
	
	if( when > 0 ) {
		DelayedMessage dm( last_time + when, message );
		delayed_messages.append( dm );
	} else {
		queue.in( message );
	}
}

const Message *MessageQueue::out()
{
	return queue.out();
}

void MessageQueue::time_elapsed()
{
	++last_time;
	int i = 0;
	while( i < delayed_messages.numElements() ) {
		if( delayed_messages[i].when <= last_time ) {
			queue.in( delayed_messages[i].message );
			delayed_messages.removeIndex( i );
		} else {
			i++;
		}
	}
}
