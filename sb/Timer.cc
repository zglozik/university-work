
#include	"debug.h"
#include	"Timer.h"
#include	"sb.h"

#define	TIME	1

Timer::Timer( const char *name ) : Object( name )
{
LOG( "Timer::Timer" );

_last_time = time( NULL );
}

void Timer::handleMessage( const Message &message )
{
LOG( "Timer::handleMessage" );

switch( message.messageId() ) {
    case SET_TIMER:
	setTimer( *(setTimerParam *) message.data() );
	break;
}
}

bool Timer::scheduling()
{
	time_t	current_time = time( NULL );

	if( current_time < _last_time + TIME )
		return false;
	_last_time = current_time;
	print( "Eltelt 1 perc." );
	for( int i=0; i<_list.numElements(); ) {
		if( _list[i]._when<=_last_time ) {
			message_queue->in( *_list[i]._message );
			delete _list[i]._message;
			_list.removeIndex( i );
		} else {
			i++;
		}
	}
	return false;
}

void Timer::setTimer( const setTimerParam &param )
{
	LOG( "Timer::setTimer" );

	_list.append( setTimerParam(param._when * TIME + _last_time,
		     param._message) );
}
