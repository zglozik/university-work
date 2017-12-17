
#include	"stdtypes.h"
#include	"MessageQueue.h"
#include	"global.h"
#include	<time.h>
#include	<stdlib.h>

int main() {

	pvm_mytid();

	srand( time( NULL ) );

	manager = NewTask( "Manager", "Szervezo" );
	waiting_queue = NewTask( "WaitingQueue", "Sor" );
	assistant = NewTask( "Assistant", "Elado" );
	store = NewTask( "Store", "Raktar" );
	SendGlobal( manager );
	SendGlobal( waiting_queue );
	SendGlobal( assistant );
	SendGlobal( store );

	pvm_exit();

	return 0;
}
