
#ifndef	WAITING_QUEUE_H
#define	WAITING_QUEUE_H

#include	"stdtypes.h"
#include	"MessageQueue.h"

class Person;

class WaitingQueue : public Object {

    public:

	WaitingQueue( const char *name, int tid, int num_queues = 1 );
	~WaitingQueue();

	void handleMessage( const Message &message );

    private:
	void enterQueue( int person );
	void leaveQueue( int queue_index );

    private:
	Queue< int >	*_persons;
	int		_num_queues;

};

#endif /* WaitingQueue */
