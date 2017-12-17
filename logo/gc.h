
#ifndef	GC_H
#define	GC_H

#include	"new.h"
#include	"iostream.h"

/*----------------------------------------------------------------------
 *	Class Reference
 *----------------------------------------------------------------------*/

class Reference {

    public:
	Reference() : _references(0), _array(_static_array) { }
	Reference( const Reference & ) : 
		_references(0), _array(_static_array) { }

	void	*operator new(size_t size) 
	{
		_static_array = false;
		return new char [size];
	}

	void	*operator new[](size_t size) 
	{
		_static_array = true;
		return new char [size];
	}

	Reference &operator =( const Reference & ) { return *this; }
	int references() const { return _references; }
	bool is_array() const { return _array; }
virtual	void reference() const { ++((Reference *) this)->_references; }
virtual	void dereference() const { --((Reference *) this)->_references; }
 
    private:
	int	_references;
	bool	_array;
static	bool	_static_array;
};


/*----------------------------------------------------------------------
 *	Ptr template class
 *----------------------------------------------------------------------*/

template< class T >
class Ptr {

    public:
	Ptr( T *p = NULL );
	Ptr( const Ptr &ptr );
virtual	~Ptr();

inline	Ptr &operator = ( const Ptr &b );
inline	Ptr &operator = ( T *b );
inline	bool operator == ( const Ptr &b ) const;
inline	T *operator ->() const;
inline	T &operator *() const;
inline	operator T *() const;
inline	T &operator []( int ) const;

    private:
	inline	void change_ptr( T * );
	
    private:
	T	*_ptr;

};

template< class T >
Ptr<T>::Ptr( T *p = NULL )
{
//	cerr << "constructor Ptr( T * )\n";

	_ptr = p;
	if( p ) _ptr->reference();
}

template< class T >
Ptr<T>::Ptr( const Ptr &ptr )
{
//	cerr << "constructor Ptr( const Ptr & )\n";

	_ptr = ptr._ptr;
	if( _ptr ) _ptr->reference();
}

template< class T >
Ptr<T>::~Ptr()
{
//	cerr << "destructor ~Ptr \n";

	change_ptr( NULL );
}

template< class T >
inline	Ptr< T > &Ptr< T >::operator = ( const Ptr &b )
{
//	cerr << "operator =\n";

	if( _ptr == b._ptr ) return *this;
	change_ptr( b._ptr );

	return *this;
}

template< class T >
inline	Ptr< T > &Ptr< T >::operator = ( T *b )
{
//	cerr << "operator =\n";

	if( _ptr == b ) return *this;
	change_ptr( b );

	return *this;
}

template< class T >
inline	bool Ptr< T >::operator == ( const Ptr &b ) const
{
	return _ptr == b._ptr;
}

template< class T >
inline	T *Ptr< T >::operator ->() const
{
	if( !_ptr ) {
		cerr << "NULL pointer exception.";
		exit( 1 );
	}
	return _ptr;
}

template< class T >
inline	T &Ptr< T >::operator *() const
{
	if( !_ptr ) {
		cerr << "NULL pointer exception.";
		exit( 1 );
	}
	return *_ptr;
}

template< class T >
inline	Ptr< T >::operator T *() const
{
	return _ptr;
}

template< class T >
inline	T &Ptr< T >::operator []( int index ) const
{
	if( !_ptr ) {
		cerr << "NULL pointer exception.";
		exit( 1 );
	}
	return _ptr[ index ];
}

template< class T >
inline	void Ptr< T >::change_ptr( T *p )
{
	if( _ptr ) {
		_ptr->dereference();
		if( _ptr->references() == 0 ) {
			if( _ptr->is_array() ) {
				delete [] _ptr;
			} else {
				delete _ptr;
			}
		}
	}
	_ptr = p;
	if( _ptr ) _ptr->reference();
}

/*----------------------------------------------------------------------
 *	Class RPtr
 *----------------------------------------------------------------------*/

template< class T >
class RPtr : public Ptr< T >, virtual public Reference {

    public:
	RPtr( T *p = NULL ) : Ptr< T >( p ) { }
	RPtr( Ptr< T > p ) : Ptr< T >( p ) { }
};

#endif /* GC_H */
