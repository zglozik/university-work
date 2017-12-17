
#include	"debug.h"
#include	"Manager.h"
#include	<stdlib.h>
#include	<iostream.h>
#include	<unistd.h>

#define	INIT_FILE	"usr/psb/manager.in"

Manager::Manager( const char *name, int tid ) : Object( name, tid )
{
	LOG( "Manager::Manager" );

	char buf[100];
	cout << getcwd(buf, 100) << '\n';
	if( (_finit = fopen( INIT_FILE, "r" )) == NULL ) {
		fprintf( stderr, "can't open file: %s\n", INIT_FILE );
		exit( 1 );
	}
	_eof = fgets( _buf, sizeof(_buf), _finit ) == NULL;

	message_queue.in( Message(id(), NEW_PERSON) );
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
	}
}

void Manager::newPerson()
{
	LOG( "Manager::newPerson" );

	int	when = 0;

	if( !_eof ) {
		char	name[50];
		
		parseLine( _buf, name, when );
		int tid = NewTask( "Person", name );
		SendGlobal( tid );
		_eof = fgets( _buf, sizeof(_buf), _finit ) == NULL;
		message_queue.in( Message(id(), NEW_PERSON) );
	}
}

void Manager::parseLine( char *line, char *name, int &when )
{

	line = strtok( line, "\t" );
	strcpy( name, line );
	line = strtok( NULL, "\t" );
	when = atoi( line );
}

int main()
{
	MainLoop<Manager> a;
	
	a();
}
