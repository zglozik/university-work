
#ifndef	_TRANSFORM_HH_
#define	_TRANSFORM_HH_

#include	"debug.hh"
#include	"vector.hh"

template< class ELEM >
class Transform {

friend	Transform operator * ( const Transform &a, const Transform &b );
friend	ostream &operator << ( ostream &s, const Transform &elem );
			
// ctors & dtor
    public:
	Transform(); // makes the identity transformation

// opers

    public:
	Vector<ELEM>	transform( const Vector<ELEM> &p ) const;

	Transform	&rotate( double degree, char axes );
	Transform	&transfer( const ELEM &x, 
				  const ELEM &y, 
				  const ELEM &z );
	Transform	&scale( double sx, double sy, double sz );

	Transform	&set( ELEM (*data)[4] );
		
// private data

    private:
	ELEM	matrix[4][4];

};

template< class ELEM >
Transform<ELEM> operator * ( const Transform<ELEM> &a, 
			    const Transform<ELEM> &b );

template< class ELEM >
ostream &operator << ( ostream &s, const Transform<ELEM> &elem );

//	include implementation

#include	"transform.cc"

#endif				// _TRANSFORM_HH_
