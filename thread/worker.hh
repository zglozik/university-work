#ifndef	_WORKER_HH_
#define	_WORKER_HH_

#include	"thread.hh"
#include	"mutex.hh"

class Worker : public Thread {

public:
	Worker( int index, Mutex *const left, Mutex *const right );
	void	*run();

protected:
	int	_index;
	Mutex	*_left, *_right;

};

#endif 
