

#include	"af.h"
#include	"Object.h"
#include	"Buyer.h"
#include	<iostream.h>
#include	<stdio.h>

MessageQueue	message_queue;
Equerry		equerry;
Owner		owner;

int main()
{
	Buyer	buyer;
	bool	quit = false;

	cout << "Az allatfarm elkezd uzemelni...\n";

	while( !quit ) {
		const Message	*message = message_queue.out();
		if( !message ) {
			char	buf[100];
			cout << "nyomj enter-t...";
			fgets( buf, 100, stdin );
			quit = buf[0] == 'q';
			message_queue.time_elapsed();
		} else {
			message->receiver().handle_message( *message );
			delete message;
		}
	}
	cout << "Az allatfarmot felszamoltak...\n";

	return 0;
}
