
#ifndef	_MUTEX_HH_
#define	_MUTEX_HH_

#include	<pthread.h>

// ---------------------------------------------------------------
//	Mutex class interface
// ---------------------------------------------------------------

class Mutex {

private:
	Mutex( const Mutex & );
	Mutex &operator = ( const Mutex & );

public:
	Mutex();
	~Mutex();

	void	P();
	void	V();

protected:
	pthread_mutex_t	_mutex;

};

// ---------------------------------------------------------------
//	Locker class interface
// ---------------------------------------------------------------

class Locker {

private:
	Locker( const Mutex & );
	Locker &operator = ( const Locker & );

public:
	Locker( Mutex *mutex );
	~Locker();

protected:
	 Mutex	*_mutex;
};

#endif
