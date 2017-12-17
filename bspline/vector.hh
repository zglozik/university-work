
//
//	Simple 3D vector class
//

#ifndef	_VECTOR_HH_
#define	_VECTOR_HH_

#include	"debug.hh"
#include	<math.h>
#include	<iostream>

template< class ELEM >
class Vector {

friend ostream &operator << <>( ostream &s, const Vector<ELEM> &v );
friend istream &operator >> <>( istream &s, Vector<ELEM> &v );
friend bool operator < <>( const Vector<ELEM> &a, const Vector<ELEM> &b );
friend bool operator == <>( const Vector<ELEM> &a, const Vector<ELEM> &b );
friend Vector<ELEM> operator + <>( const Vector<ELEM> &a, 
				  const Vector<ELEM> &b );
friend Vector<ELEM> operator + <>( const Vector<ELEM> &a );
friend Vector<ELEM> operator - <>( const Vector<ELEM> &a,
				  const Vector<ELEM> &b );
friend Vector<ELEM> operator - <>( const Vector<ELEM> &a );
friend ELEM operator * <>( const Vector<ELEM> &a, const Vector<ELEM> &b );
friend Vector<ELEM> operator * <>( const ELEM &a, const Vector<ELEM> &b );
friend Vector<ELEM> operator / <>( const Vector<ELEM> &a, const ELEM &b );

// ctors & dtor

    public:
	Vector() { }
	Vector( int null ) : x(0), y(0) { assert( null == 0 ); }
	Vector( const ELEM &x, const ELEM &y )
		: x( x ), y( y ) { }

// opers
	
    public:
	ELEM  &operator [] ( int i )
	{
		ELEM	*elements[] = { &x, &y };

		TEST_EXPR( i >= 0 && i <= 1 );
		return *elements[i];
	}

	const ELEM &operator [] ( int i ) const
	{
		return (const ELEM &) (((Vector *) this)->operator []( i ));
	}

	Vector &operator += ( const Vector &b )
	{
		x += b.x;
		y += b.y;
		return *this;
	}

	Vector	&operator -= ( const Vector &b )
	{
		x -= b.x;
		y -= b.y;
		return *this;
	}

// private data

    private:
	ELEM	x, y;

};

//	include implementation

#include	"vector.cc"

#endif _VECTOR_HH_		// _VECTOR_HH_
