
#include	<pvm3.h>
#include	"MessageQueue.h"

void MessageQueue::in( const Message &message )
{
	int i;

	pvm_initsend( PvmDataDefault );
	i = message.receiver();
	pvm_pkint( &i, 1, 1 );
	i = message.messageId();
	pvm_pkint( &i, 1, 1 );
	i = message.size();
	pvm_pkint( &i, 1, 1 );
	if( i>0 ) {
		pvm_pkbyte( (char *)message.data(), i, 1 );
	}
	pvm_send( message.receiver(), 0 );
}

Message *MessageQueue::out()
{
	int	n, receiver, id;
	
	pvm_recv( -1, -1 );
	pvm_upkint( &receiver, 1, 1 );
	pvm_upkint( &id, 1, 1 );
	pvm_upkint( &n, 1, 1 );
	char *buf = NULL;
	if( n > 0 ) {
		buf = new char[ n ];
		pvm_upkbyte( buf, n, 1 );
	}


	Message *msg = new Message( receiver, id, buf, n );
	delete [] buf;

	return msg;
}

