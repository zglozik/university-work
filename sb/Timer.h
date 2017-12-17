
#ifndef	TIMER_H
#define	TIMER_H

#include	"stdtypes.h"
#include	"Object.h"

#include	<time.h>

class Timer : public Object {

    public:
	enum Messages { SET_TIMER };

	struct setTimerParam {
		setTimerParam( int when, const Message *message ) :
			_when( when ), _message( message ) { }

		int		_when;
		const Message	*_message;
	};

	Timer( const char *name );

	void handleMessage( const Message &message );
	bool scheduling();
	bool isPendingMessage() const { return _list.numElements() > 0; }

    private:
	void setTimer( const setTimerParam &param );

    private:
	List< setTimerParam >	_list;
	time_t			_last_time;

};

#endif /* TIMER_H */
