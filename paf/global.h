
#ifndef	GLOBAL_H
#define	GLOBAL_H

#include	<stdio.h>
#include	"MessageQueue.h"

class MessageQueue;

extern MessageQueue	message_queue;
extern int		equerry;
extern int		owner;
extern int		buyer;
extern char		*animal_names[];

// Owner messages

enum { GIVE_ANIMAL, CREATE_ANIMAL, ASK_FOR_ANIMAL, MUST_FEED };

// Animals messages

enum { EAT, IS_HUNGRY };

#define	NUM_ANIMALS	2

enum AnimalType { CHICKEN, PIG };

// Equerry messages

enum {
	ADD_ANIMAL, GET_ANIMAL, FEED, COUNT_HUNGRY_ANIMALS,
	HUNGRY_ANSWER
};

struct AnimalData {
	int		animal;
	AnimalType	type;
};

struct HungryAnswerParam {
	bool		answer;
	AnimalType	animal;
};

// Buyer messages

enum { BUY };

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
		pvm_upkint( &equerry, 1, 1 );
		pvm_upkint( &owner, 1, 1 );
		pvm_upkint( &buyer, 1, 1 );

		T *obj = new T( buf, tid );
		while( 1 ) {
			Message *msg = message_queue.out();
			obj->handle_message( *msg );
			delete msg;
		}
	}

};

#endif /* GLOBAL_H */
