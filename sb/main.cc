
#include	"stdtypes.h"
#include	"sb.h"
#include	"Manager.h"
#include	<time.h>
#include	<stdlib.h>

Queue< Message >	*message_queue;
Timer			*timer;
Manager			*manager;
WaitingQueue		*waiting_queue;
Assistant		*assistant;
Store			*store;

int main() {

	srand( time( NULL ) );

	message_queue = new Queue< Message >;
	timer = new Timer( "Idozito" );
	manager = new Manager( "Szervezo" );
	waiting_queue = new WaitingQueue( "Sor" );
	assistant = new Assistant( "Elado" );
	store = new Store( "Raktar" );

	timer->scheduling();
	Message	*message = message_queue->out();
	while( message || timer->isPendingMessage() ) {
		if( message ) {
			message->receiver().handleMessage( *message );
			delete message;
		}
		timer->scheduling();
		message = message_queue->out();
	}

	delete store;
	delete manager;
	delete timer;
	delete	message_queue;

	return 0;
}
