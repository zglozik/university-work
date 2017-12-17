
#include	"Manager.h"
#include	"Person.h"
#include	"sb.h"
#include	"debug.h"
#include	"Timer.h"
#include	<stdlib.h>

#define	INIT_FILE	"manager.in"

Manager::Manager( const char *name ) : Object( name )
{
	LOG( "Manager::Manager" );

	if( (_finit = fopen( INIT_FILE, "r" )) == NULL ) {
		fprintf( stderr, "can't open file: %s\n", INIT_FILE );
		exit( 1 );
	}
	_eof = fgets( _buf, sizeof(_buf), _finit ) == NULL;

	message_queue->in( Message(this, NEW_PERSON) );
}

Manager::~Manager()
{
	LOG( "Manager::~Manager" );

	fclose( _finit );
}

void Manager::handleMessage( const Message &message )
{
	LOG( "Manager::handleMessage" );

	switch( message.messageId() ) {
	    case NEW_PERSON:
		newPerson();
		break;
	    case DIE_PERSON:
		diePerson( *(Person **) message.data() );
		break;
	}
}

void Manager::newPerson()
{
	LOG( "Manager::newPerson" );

	int	when = 0;

	if( !_eof && when == 0 ) {
		char	name[50];
		
		parseLine( _buf, name, when );
		new Person( name );
		_eof = fgets( _buf, sizeof(_buf), _finit ) == NULL;
	}
	if( !_eof ) {
		Timer::setTimerParam tp( when, new Message(this, NEW_PERSON) );
		message_queue->in( Message(timer, Timer::SET_TIMER,
					   &tp, sizeof(tp)) );
	}
}

void Manager::diePerson( Person *person )
{
	LOG( "Manager::diePerson" );

	delete person;
}

void Manager::parseLine( char *line, char *name, int &when )
{

	line = strtok( line, "\t" );
	strcpy( name, line );
	line = strtok( NULL, "\t" );
	when = atoi( line );
}
