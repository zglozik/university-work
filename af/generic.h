
#ifndef	GENERIC_H
#define	GENERIC_H

#ifndef	NULL
#define	NULL	0
#endif

#define	ARRAY_SIZE( arr )	(sizeof(arr) / sizeof(arr[0]))

/*----------------------------------------------------------------------
 *	QUEUE template
 *----------------------------------------------------------------------*/

template< class ELEM >
class Queue {

    public:
	Queue();
	virtual ~Queue();

	void in( const ELEM &elem );
	ELEM *out();
	ELEM *head();
	bool isEmpty() const { return !_first; }
	int numElements() const { return _num_elements; }

    private:
	Queue( const Queue & );
	Queue &operator =( const ELEM & );

    private:
	struct Data {
		struct Data	*next;
		ELEM		*elem;
	};
	
	Data	*_first, *_last;
	int	_num_elements;

};

template< class ELEM >
Queue< ELEM >::Queue()
{
	_first = _last = NULL;
	_num_elements = 0;
}

template< class ELEM >
Queue< ELEM >::~Queue()
{
	while( !isEmpty() ) {
		delete out();
	}
}

template< class ELEM >
void Queue< ELEM >::in( const ELEM &elem )
{
	Data	*temp = new Data;
	temp->elem = new ELEM( elem );
	temp->next = NULL;

	if( !_first ) {
		_first = _last = temp;
	} else {
		_last->next = temp;
		_last = temp;
	}
	_num_elements++;
}

template< class ELEM >
ELEM *Queue< ELEM >::out()
{

	if( !_first )
		return NULL;
	Data	*temp = _first;
	ELEM	*rv = _first->elem;
	_first = _first->next;
	delete temp;
	_num_elements--;

	return rv;
}

template< class ELEM >
inline ELEM *Queue< ELEM >::head()
{
	return _first ? _first->elem : (ELEM *) NULL;
}

/*----------------------------------------------------------------------
 *	LIST template
 *----------------------------------------------------------------------*/

template< class ELEM >
class List {

    public:
	List( int min_size = 0, int step_size = 5 );
	virtual ~List();

	void append( const ELEM &elem );
	inline ELEM &operator []( int i ) const;
	const int numElements() const { return _num_elements; }
	bool isEmpty() const { return _num_elements == 0; }
	void removeIndex( int index );
	void remove( const ELEM &elem );

    private:
	List &operator =( const List & );
	List( const List & );

    private:
	int	_mem_size;
	int	_step_size;
	int	_num_elements;
	ELEM	**_elements;

};

template< class ELEM >
List< ELEM >::List( int min_size = 0, int step_size = 5 )
{
	_mem_size = min_size;
	_step_size = step_size;
	_num_elements = 0;
	_elements = min_size > 0 ? new ELEM *[min_size] : (ELEM **) NULL;
}

template< class ELEM >
List< ELEM >::~List()
{
	for( int i=0; i<_num_elements; i++ )
		delete _elements[i];
	delete [] _elements;
}

template< class ELEM >
void List< ELEM >::append( const ELEM &elem )
{
	if( _num_elements == _mem_size ) {
		ELEM	**temp = new ELEM *[_mem_size += _step_size];
		memcpy( temp, _elements, sizeof(ELEM *) * _num_elements );
		delete [] _elements;
		_elements = temp;
	}
	_elements[ _num_elements++ ] = new ELEM( elem );
}

template< class ELEM >
inline ELEM &List< ELEM >::operator []( int i ) const
{
	return *_elements[i];
}

template< class ELEM >
void List< ELEM >::remove( const ELEM &elem )
{
	for( int i=0; i<_num_elements; ) {
		if( *_elements[i] == elem ) {
			removeIndex( i );
		} else {
			i++;
		}
	}
}

template< class ELEM >
void List< ELEM >::removeIndex( int i )
{
	delete _elements[i];
	_num_elements--;
	memmove( &_elements[i], &_elements[i+1],
		(_num_elements-i) * sizeof(ELEM *) );
}

#endif /* GENERIC_H */
