
#ifndef	MESSAGE_QUEUE_H
#define	MESSAGE_QUEUE_H

#include	"generic.h"
#include	"Object.h"
#include	<time.h>

class MessageQueue {

    public:
	MessageQueue();
    private:
	MessageQueue( const MessageQueue & ) {}

    public:
	void in( const Message &message, int when = 0 );
	const Message *out();
	void time_elapsed();
	bool is_empty()
	{
		return queue.isEmpty() && delayed_messages.numElements() == 0;
	}

    private:

	struct DelayedMessage {
		DelayedMessage( const int awhen, const Message amessage ) :
			when(awhen), message(amessage) { }

		const int	when;
		const Message	message;
	};

	Queue< Message >	queue;
	List< DelayedMessage >	delayed_messages;
	time_t			last_time;

};

#endif /* MESSAGE_QUEUE */
