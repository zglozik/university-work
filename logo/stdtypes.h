
#ifndef	STDTYPES_H
#define	STDTYPES_H

#include	<assert.h>
#include	"gc.h"

#define	ARRAY_SIZE( arr )	(sizeof(arr) / sizeof(arr[0]))

/*----------------------------------------------------------------------
 *	DQueue template
 *----------------------------------------------------------------------*/

template< class ELEM >
class DQueue : virtual public Reference {

    public:
	class EMPTY : virtual public Reference { };

	DQueue();
	DQueue( const DQueue & );

	void lin( const ELEM &elem );
	ELEM lout() throw( EMPTY );
	ELEM &lhead() throw( EMPTY );
	void rin( const ELEM &elem );
	ELEM rout() throw( EMPTY );
	ELEM &rhead() throw( EMPTY );
	bool is_empty() const { return !_first; }
	int num_elements() const { return _num_elements; }

    private:
	DQueue &operator =( const ELEM & );

    private:
	struct Data : virtual public Reference {
		Data( Ptr< struct Data > _next, const ELEM &_elem ) :
			next( _next ), elem( _elem ) { }
		Ptr< struct Data >	next;
		ELEM			elem;
	};
	
	Ptr< Data >	_first, _last;
	int		_num_elements;

};

template< class ELEM >
DQueue< ELEM >::DQueue() : _num_elements(0)
{
}

template< class ELEM >
DQueue< ELEM >::DQueue( const DQueue &b ) : _num_elements(0)
{
	Ptr< Data > temp = b._first;

	while( temp != NULL ) {
		rin( temp->elem );
		temp = temp->next;
	}
}

template< class ELEM >
void DQueue< ELEM >::rin( const ELEM &elem )
{
	Ptr< Data >	temp = new Data( NULL, elem );

	if( !_first ) {
		_first = _last = temp;
	} else {
		_last->next = temp;
		_last = temp;
	}
	_num_elements++;
}

template< class ELEM >
ELEM DQueue< ELEM >::rout() throw( EMPTY )
{
	if( !_first ) throw EMPTY();

	ELEM rv = _last->elem;
	if( _first == _last ) {
		_first = _last = NULL;
	} else {
		Ptr< Data > temp = _last;
		_last = _first;
		while( _last->next != temp ) _last = _last->next;
	}
	_num_elements--;

	return rv;
}

template< class ELEM >
inline ELEM &DQueue< ELEM >::rhead() throw( EMPTY )
{
	if( !_first ) throw EMPTY();

	return _last->elem;
}

template< class ELEM >
void DQueue< ELEM >::lin( const ELEM &elem )
{
	Ptr< Data > temp = new Data( _first, elem );

	if( !_first ) {
		_last = temp;
	}
	_first = temp;

	_num_elements++;
}

template< class ELEM >
ELEM DQueue< ELEM >::lout() throw( EMPTY )
{
	if( !_first ) throw EMPTY();

	ELEM rv = _first->elem;
	_first = _first->next;
	_num_elements--;

	return rv;
}

template< class ELEM >
inline ELEM &DQueue< ELEM >::lhead() throw( EMPTY )
{
	if( !_first ) throw EMPTY();

	return _first->elem;
}

/*----------------------------------------------------------------------
 *	Array template
 *----------------------------------------------------------------------*/

template< class ELEM >
class Array : virtual public Reference {

    public:
	Array( int min_size = 0, int step_size = 5 );
	Array( const Array & );
	~Array();

	void append( const ELEM &elem );
	inline ELEM &operator []( int i ) const;
	const int num_elements() const { return _num_elements; }
	void remove_index( int index );
	void remove( const ELEM &elem );

	Array &operator =( const Array &b )
	{
		if( this == &b ) return *this;

		delete [] _elements;
		_mem_size = 0;
		_step_size = b._step_size;
		_num_elements = 0;
		_elements = NULL;

		for( int i = 0; i < b._num_elements; i++ ) {
			append( b[i] );
		}
		return *this;
	}

    private:
	int	_mem_size;
	int	_step_size;
	int	_num_elements;
	ELEM	*_elements;
};

template< class ELEM >
Array< ELEM >::Array( int min_size = 0, int step_size = 5 )
{
	_mem_size = min_size;
	_step_size = step_size;
	_num_elements = 0;
	_elements = min_size > 0 ? new ELEM[min_size] : 0;
}

template< class ELEM >
Array< ELEM >::~Array()
{
	delete [] _elements;
}

template< class ELEM >
void Array< ELEM >::append( const ELEM &elem )
{
	if( _num_elements == _mem_size ) {
		ELEM *temp = new ELEM[_mem_size += _step_size];

		for( int i = 0; i < _num_elements; i++ )
			temp[i] = _elements[i];
		delete [] _elements;
		_elements = temp;
	}
	_elements[ _num_elements++ ] = elem;
}

template< class ELEM >
inline ELEM &Array< ELEM >::operator []( int i ) const
{
	assert( i>=0 && i<_num_elements );

	return _elements[i];
}

template< class ELEM >
void Array< ELEM >::remove( const ELEM &elem )
{
	for( int i=0; i<_num_elements; ) {
		if( _elements[i] == elem ) {
			remove_index( i );
		} else {
			i++;
		}
	}
}

template< class ELEM >
void Array< ELEM >::remove_index( int i )
{
	assert( i>=0 && i<_num_elements );

	for( int j = i+1; j < _num_elements; j++ )
		_elements[j-1] = _elements[j];
	_num_elements--;
}

/*----------------------------------------------------------------------
 *	Stack template
 *----------------------------------------------------------------------*/

template< class ELEM >
class Stack : public DQueue< ELEM > {

    public:
	class EMPTY : virtual public Reference { };

	Stack() { }
	Stack( const Stack &b ) : DQueue<ELEM>( b ) { }

	void	push( const ELEM &elem ) { rin( elem ); }
	ELEM	pop() throw( EMPTY );
	ELEM	top() throw( EMPTY );
	int	num_elements() { return DQueue<ELEM>::num_elements(); }
	bool	is_empty() { return num_elements() == 0; }
};

template< class ELEM >
ELEM Stack<ELEM>::pop() throw( EMPTY )
{
	if( num_elements() == 0 ) throw EMPTY();
	return rout();
}

template< class ELEM >
ELEM Stack<ELEM>::top() throw( EMPTY )
{
	if( num_elements() == 0 ) throw EMPTY();
	return rhead();
}

#endif /* STDTYPES_H */
