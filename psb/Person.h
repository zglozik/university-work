
#ifndef	PERSON_H
#define	PERSON_H

#include	"MessageQueue.h"
#include	"stdtypes.h"

class Person : public Object {

    public:

	Person( const char *name, int tid );
	~Person();

	void handleMessage( const Message &message );

    private:
	void enter();
	void wait( int queue_index );
	void firstInQueue();
	void assistantReady();
	void anyMore();
	void pay( int money );
	void paid();
	void eat();
	void exit();

	void order();

    private:
	int		_queue_index;

	Queue< int >	*_orders;

};

#endif /* PERSON_H */

