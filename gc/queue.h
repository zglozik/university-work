
#ifndef	QUEUE_H
#define	QUEUE_H

#include	"gc.h"

#include	<assert.h>
#include	<iostream.h>

#ifdef DEBUG
#define	TEST_EXPR( expr )	assert( expr )
#else
#define	TEST_EXPR( expr )	;
#endif

/*----------------------------------------------------------------------
 *	QUEUE template
 *----------------------------------------------------------------------*/

template< class ELEM >
class Queue : virtual public Reference {

friend	inline ostream &operator << ( ostream &, const Queue & );
friend	inline istream &operator >> ( istream &, Queue & );

    public:
	Queue();
 	Queue( const Queue & );

	Queue &operator =( const Queue &q2 );
	Queue &in( const ELEM &elem );
	ELEM out();
	ELEM &head();
	bool isEmpty() const { return !_first; }
	int numElements() const { return _num_elements; }

    private:
	struct Data : virtual public Reference {
		Data( const ELEM &elem2 ) : next(0), elem(elem2) { }

		Ptr< Data >	next;
		ELEM		elem;
	};
	
	Ptr< Data >	_first, _last;
	int		_num_elements;

};

template< class ELEM >
Queue< ELEM >::Queue() : _first(0), _last(0), _num_elements(0)
{
}

template< class ELEM >
Queue< ELEM >::Queue( const Queue &queue ) 
	: _first(0), _last(0), _num_elements(0)
{
	*this = queue;
}

template< class ELEM >
Queue< ELEM > &Queue< ELEM >::operator = ( const Queue &q2 )
{
	if( this == &q2 ) return *this;

	_first = _last = NULL; // this will free up the allocated memory
	_num_elements = 0;
	Ptr< Data > temp = q2._first;
	while( temp != NULL ) {
		in( temp->elem );
		temp = temp->next;
	}

	return *this;
}

template< class ELEM >
Queue< ELEM > &Queue< ELEM >::in( const ELEM &elem )
{
	Ptr< Data > temp = new Data( elem );

	if( !_first ) {
		_first = _last = temp;
	} else {
		_last->next = temp;
		_last = temp;
	}
	_num_elements++;

	return *this;
}

template< class ELEM >
ELEM Queue< ELEM >::out()
{
	TEST_EXPR( _first != NULL );

	Ptr< Data > temp = _first;
	_first = _first->next;
	_num_elements--;

	return temp->elem;
}

template< class ELEM >
inline ELEM &Queue< ELEM >::head()
{
	TEST_EXPR( _first != NULL );

	return _first->elem;
}

template< class ELEM >
bool Queue< ELEM >::isEmpty() const
{
	return !_first;
}

template< class ELEM >
int Queue< ELEM >::numElements() const
{
	return _num_elements;
}

template< class ELEM >
inline ostream &operator <<( ostream &out, const Queue< ELEM > &queue )
{
	out << queue.numElements() << endl;
	Ptr< Queue< ELEM >::Data > temp = queue._first;
	while( temp != NULL ) {
		out << temp->elem;
		temp = temp->next;
		if( temp ) out << '\t';
	}
	return out;
}

template< class ELEM >
inline istream &operator >>( istream &in, Queue< ELEM > &queue )
{
	int num = 0;
	in >> num;
	for( int i = 0; i < num; i++ ) {
		ELEM elem;
		in >> elem;
		queue.in( elem );
	}
	return in;
}

#endif /* QUEUE_H */
