
#include	"global.h"
#include	<stdio.h>

MessageQueue	message_queue;
int		equerry;
int		owner;
int		buyer;

char *animal_names[] = { "csirke", "diszno" };

int NewTask( char *file, char *name )
{
	int	tid;
	
	int n = pvm_spawn( file, NULL, PvmTaskDefault, NULL, 1, &tid );
	if( n != 1 ) {
		fprintf( stderr, "can't start task: %s\n", file );
		exit( 1 );
	}
	pvm_initsend( PvmDataDefault );
	pvm_pkstr( name );
	pvm_send( tid, 0 );

	return tid;
}

void SendGlobal( int tid )
{
	pvm_initsend( PvmDataDefault );
	pvm_pkint( &equerry, 1, 1 );
	pvm_pkint( &owner, 1, 1 );
	pvm_pkint( &buyer, 1, 1 );
	pvm_send( tid, 0 );
}
