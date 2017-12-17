
#ifndef	GLOBAL_H
#define	GLOBAL_H

#include	"MessageQueue.h"

extern MessageQueue	message_queue;
extern int		manager;
extern int		waiting_queue;
extern int		assistant;
extern int		store;

#define	NUM_FOOD	7

enum Food {
	HAMBURGER, CHICKEN, PIZZA, COKE, SPRITE, FANTA, TONIC
};

extern char	*food_names[];
extern int	food_prices[];

enum {
	ARE_YOU_READY, ORDER, ENOUGH, GIVE_MONEY,
	ADD_FOOD
};

struct AssistantOrderParam {
	AssistantOrderParam( Food what, int number )
	{
		_what = what;
		_number = number;
	}

	Food	_what;
	int	_number;
};

struct AssistantAddFoodParam {
	AssistantAddFoodParam( Food what, int number )
	{
		_what = what;
		_number = number;
	}

	Food	_what;
	int	_number;
};

enum { NEW_PERSON, DIE_PERSON };

enum {
	ENTER, WAIT, FIRST_IN_QUEUE, ASSISTANT_READY, ANY_MORE,
	PAY, PAID, EAT, EXIT
};

enum { MAKE_FOOD };

enum {
	ENTER_QUEUE, LEAVE_QUEUE
};

/*----------------------------------------------------------------------
 *	Functions
 *----------------------------------------------------------------------*/

int  NewTask( char *file, char *name );
void SendGlobal( int tid );

template<class T>
class MainLoop {

    public:
	void operator ()()
	{
		int	tid = pvm_mytid();
		char	buf[ 100 ];
		
		if( tid<0 ) {
			fprintf( stderr, "can't get tid\n" );
			exit( 1 );
		}
		
		pvm_recv( -1, -1 );
		pvm_upkstr( buf );
		pvm_recv( -1, -1 );
		pvm_upkint( &manager, 1, 1 );
		pvm_upkint( &waiting_queue, 1, 1 );
		pvm_upkint( &assistant, 1, 1 );
		pvm_upkint( &store, 1, 1 );

		T *obj = new T( buf, tid );
		while( 1 ) {
			Message *msg = message_queue.out();
			obj->handleMessage( *msg );
			delete msg;
		}
	}

};

#endif /* GLOBAL_H */
