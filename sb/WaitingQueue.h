
#ifndef	WAITING_QUEUE_H
#define	WAITING_QUEUE_H

#include	"stdtypes.h"
#include	"Object.h"

class Person;

class WaitingQueue : public Object {

    public:
	enum messages {
		ENTER_QUEUE, LEAVE_QUEUE
	};

	WaitingQueue( const char *name, int num_queues = 1 );
	~WaitingQueue();

	void handleMessage( const Message &message );

    private:
	void enterQueue( Person *person );
	void leaveQueue( int queue_index );

    private:
	Queue< Person * >	*_persons;
	int			_num_queues;

};

#endif /* WaitingQueue */
