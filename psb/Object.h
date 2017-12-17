

#ifndef	OBJECT_H
#define	OBJECT_H

#include	<string.h>
#include	"debug.h"
#include	<pvm3.h>

class Object;

class Message {
	
    public:
	Message( int receiver, int message_id,
		void *data = NULL, int size = 0 )
	{
		_receiver = receiver;
		_message_id = message_id;
		_size = size;
		if( data ) {
			_data = new char[_size];
			memcpy( _data, data, _size );
		} else {
			_data = NULL;
 		}
	}

	Message( const Message &message )
	{
		_receiver = message._receiver;
		_message_id = message._message_id;
		_size = message._size;
		if( message._data ) {
			_data = new char[_size];
			memcpy( _data, message._data, _size );
		} else {
			_data = NULL;
 		}
	}

	virtual ~Message() { delete [] _data; }

	int receiver() const { return _receiver; }
	int messageId() const { return _message_id; }
	int size() const { return _size; }
	void *data() const { return _data; }

    private:
	int	_receiver;
	int	_message_id;
	char	*_data;
	int	_size;

};

class Object {

    protected:
	Object( const char *name, int id ) 
	{
		_name = strdup( name );
		_id  =id;
	}

	virtual ~Object() { delete [] _name; }

    public:
	const char *name() const { return _name; }
	const int   id() const { return _id; }
	virtual void handleMessage( const Message &message ) = 0;
	void print( const char *string ) const;

    private:
	char	*_name;
	int	_id;

};

#endif /* OBJECT_H */

