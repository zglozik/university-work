
#ifndef	OBJECT_H
#define	OBJECT_H

#include	<string.h>

class Object;

class Message {
	
    public:
	Message( Object *receiver, int message_id,
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

	Object &receiver() const { return *_receiver; }
	int message_id() const { return _message_id; }
	void *data() const { return _data; }

    private:
	Object	*_receiver;
	int	_message_id;
	char	*_data;
	int	_size;

};

class Object {

    public:
	Object( const char *name ) { _name = strdup( name ); }
	virtual ~Object() { delete [] _name; }

	const char *name() const { return _name; }
	virtual void handle_message( const Message &message ) = 0;
	void print( const char *string ) const;

    private:
	char	*_name;

};

#endif /* OBJECT_H */

