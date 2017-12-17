

#include	"Store.h"
#include	"debug.h"
#include	"global.h"
#include	<stdlib.h>

#define	INIT_FILE	"usr/psb/store.in"

Store::Store( const char *name, int tid ) : Object( name, tid )
{
	LOG( "Store::Store" );

	if( (_finit = fopen( INIT_FILE, "r" )) == NULL ) {
		fprintf( stderr, "can't open file: %s\n", INIT_FILE );
		exit( 1 );
	}
	_eof = fgets( _buf, sizeof(_buf), _finit ) == NULL;
	message_queue.in( Message( id(), MAKE_FOOD ) );
}

Store::~Store()
{
	LOG( "Store::~Store" );

	fclose( _finit );
}

void Store::handleMessage( const Message &message )
{
	LOG( "Store::handleMessage" );

	switch( message.messageId() ) {
	    case MAKE_FOOD:
		makeFood();
		break;
	}
}

void Store::makeFood()
{
	LOG( "Store::makeFood" );

	int	next = 0;

	while( !_eof && next == 0 ) {
		char	name[100];
		int	quantity;
		int	i;

		parseLine( _buf, name, quantity, next );
		for( i=0; i < NUM_FOOD && strcmp(food_names[i], name); i++ );
		if( i==NUM_FOOD ) {
			fprintf( stderr, "error in file: %s\n", INIT_FILE );
			exit(1);
		}
			
		AssistantAddFoodParam ap( (Food) i, quantity );
		message_queue.in( Message(assistant, ADD_FOOD,
					  &ap, sizeof(ap)) );
		_eof = fgets( _buf, sizeof(_buf), _finit ) == NULL;
	}
	if( !_eof ) {
		message_queue.in( Message(id(), MAKE_FOOD) );
	}
}

void Store::parseLine( char *line, char *name, int &number, int &next )
{

	line = strtok( line, "\t" );
	strcpy( name, line );
	line = strtok( NULL, "\t" );
	number = atoi( line );
	line = strtok( NULL, "\t" );
	next = atoi( line );
}

int main()
{
	MainLoop<Store> a;

	a();
}
