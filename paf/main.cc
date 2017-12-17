
#include	"generic.h"
#include	"MessageQueue.h"
#include	"global.h"
#include	<time.h>
#include	<stdlib.h>

int main() {

	pvm_mytid();

	srand( time( NULL ) );

	equerry = NewTask( "Equerry", "Istallo" );
	owner = NewTask( "Owner", "Tulajdonos" );
	buyer = NewTask( "Buyer", "Vevo" );
	SendGlobal( equerry );
	SendGlobal( owner );
	SendGlobal( buyer );

	pvm_exit();

	return 0;
}
