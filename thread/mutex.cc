
#include	"mutex.hh"

// ---------------------------------------------------------------
//	Mutex class implementation
// ---------------------------------------------------------------

Mutex::Mutex()
{
	pthread_mutex_init( &_mutex, NULL );
}

Mutex::~Mutex()
{
	pthread_mutex_unlock( &_mutex );
	pthread_mutex_destroy( &_mutex );
}

void Mutex::P()
{
	pthread_mutex_lock( &_mutex );
}

void Mutex::V()
{
	pthread_mutex_unlock( &_mutex );
}

// ---------------------------------------------------------------
//	Locker class implementation
// ---------------------------------------------------------------

Locker::Locker( Mutex *mutex )
{
	_mutex = mutex;
	_mutex->P();
}

Locker::~Locker()
{
	_mutex->V();
}
