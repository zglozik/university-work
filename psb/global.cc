
#include	"global.h"
#include	<stdio.h>

MessageQueue	message_queue;
int		manager;
int		waiting_queue;
int		assistant;
int		store;

char	*food_names[] = {
	"hamburger",
	"csirke",
	"pizza",
	"coca cola",
	"sprite",
	"fanta",
	"tonic"
};
		
int	food_prices[] = { 130, 160, 300, 50, 45, 45, 60 };

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
	pvm_pkint( &manager, 1, 1 );
	pvm_pkint( &waiting_queue, 1, 1 );
	pvm_pkint( &assistant, 1, 1 );
	pvm_pkint( &store, 1, 1 );
	pvm_send( tid, 0 );
}
