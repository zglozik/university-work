
#include	<iostream>
#include	<stdlib.h>
#include	"worker.hh"

// ---------------------------------------------------------------
//	Worker class implementation
// ---------------------------------------------------------------

static void worker_exit( void *index )
{
	cout << int(index) << ". philosopher died.\n";
}

Worker::Worker( int index, Mutex *const left, Mutex *const right )
	: _index( index ), _left( left ), _right( right )
{
}

void *Worker::run()
{
	pthread_cleanup_push( worker_exit, (void *) _index );

	cout << _index << ". philosopher is born.\n";

	while( true ) {
		pthread_testcancel();
		sleep( rand() % 10 + 2 );
		pthread_testcancel();

		_left->P();
		_right->P();

		cout << _index << ". philosopher is eating...\n";
		pthread_testcancel();
		sleep( rand() % 5 + 1 );
		pthread_testcancel();
		cout << _index << ". philosopher stopped eating.\n";

		_right->V();
		_left->V();
	}

	pthread_cleanup_pop( 1 );

	return (void *) _index;
}
