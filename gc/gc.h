
#ifndef	GC_H
#define	GC_H

#include	<new.h>
#include	<iostream.h>

/*----------------------------------------------------------------------
 *	Class Reference
 *----------------------------------------------------------------------*/

class Reference {

    public:
	Reference() : _references(0) { }
	Reference( const Reference & ) : _references(0) { }

	Reference &operator =( const Reference & ) { return *this; }
	int references() { return _references; }
virtual	void reference() { ++_references; }
virtual	void dereference() { --_references; }
 
    private:
	int	_references;
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

    private:
	inline	void change_ptr( T * );
	
	T *_ptr;
};

template< class T >
Ptr<T>::Ptr( T *p ) : _ptr(p)
{
//	cerr << "constructor Ptr( T * )\n";

	if( _ptr ) _ptr->reference();
}

template< class T >
Ptr<T>::Ptr( const Ptr &ptr ) : _ptr(ptr._ptr)
{
//	cerr << "constructor Ptr( const Ptr & )\n";

	if( _ptr ) _ptr->reference();
}

template< class T >
Ptr<T>::~Ptr()
{
//	cerr << "destructor ~Ptr\n";

	change_ptr(0);
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
inline	void Ptr< T >::change_ptr( T *p )
{
	if( _ptr ) {
		_ptr->dereference();
		if( _ptr->references() == 0 ) delete _ptr;
	}
	_ptr = p;
	if( _ptr ) _ptr->reference();
}

#endif /* GC_H */
