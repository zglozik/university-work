
#ifndef	_THREAD_HH_
#define	_THREAD_HH_

#include	<pthread.h>

// ---------------------------------------------------------------
//	Thread class interface
// ---------------------------------------------------------------

class Thread {

public:
	Thread();
	virtual ~Thread();

	int	start();
	int	join( void *&result );
	int	cancel();

	virtual void	*run() = 0;

private:
	static void	*run_st( void * );

protected:
	pthread_t	_pthread;

};

#endif
