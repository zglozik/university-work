
#include	<iostream>
#include	"worker.hh"

static const int	num_threads = 5;
static Mutex		forks[num_threads];
static Worker		*threads[num_threads];

static int start_threads( int number )
{
	int	rv = 0;

	for( int i = 0; i < number; ++i ) {
		threads[i] = 
			new Worker( i+1, &forks[i], &forks[(i+1) % number] );
		if( threads[i]->start() ) {
			cerr << "can't create thread number " << i << endl;
			rv += 1;
		}
	}

	return rv;
}

static void stop_threads( int number )
{
	void	*result;
	for( int i = 0; i < number; ++i ) {
		threads[i]->cancel();
		threads[i]->join( result );
		delete threads[i];
	}
}

int main()
{
	int rv = start_threads( num_threads );

	if( !rv ) {
		sleep( 20 );
		stop_threads( num_threads );
	}

	exit( rv );
}
