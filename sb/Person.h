
#ifndef	PERSON_H
#define	PERSON_H

#include	"Object.h"
#include	"stdtypes.h"

class Person : public Object {

    public:
	enum messages {
		ENTER, WAIT, FIRST_IN_QUEUE, ASSISTANT_READY, ANY_MORE,
		PAY, PAID, EAT, EXIT
	};

	Person( const char *name );
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

