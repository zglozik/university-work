
#include	"thread.hh"

Thread::Thread()
{
}

Thread::~Thread()
{
	cancel();
	void	*result;
	join( result );
}

int Thread::start()
{
	int rv = pthread_create( &_pthread, NULL, run_st, this );
	
	return rv;
}

int Thread::join( void *&result )
{
	int rv = pthread_join( _pthread, &result );

	return rv;
}

int Thread::cancel()
{
	int rv = pthread_cancel( _pthread );

	return rv;
}

void *Thread::run_st( void *client )
{
	Thread	*thread = (Thread *) client;

	return thread->run();
}
