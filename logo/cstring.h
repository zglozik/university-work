
#ifndef	_CSTRING_H_
#define	_CSTRING_H_

#include	"stdtypes.h"

class String {

friend	String operator + ( const String &s1, const String &s2 );
friend	bool operator < ( const String &s1, const String &s2 );
friend	bool operator == ( const String &s1, const String &s2 );

//	ctors & dtor

    public:
	String( int size = 0 );
	String( const char *s );
	String( const String &str );
virtual	~String();

//	opers

    public:
	String	&operator = ( const String &str );

	const char &operator []( int i ) const;
	int	len() const { return _len; }
	operator const char *() const { return data; }

//	private data

    private:

	char	*data;
	int	_len;
	int	size;

};

String operator + ( const String &s1, const String &s2 );

bool operator < ( const String &s1, const String &s2 );

bool operator == ( const String &s1, const String &s2 );

inline bool operator != ( const String &s1, const String &s2 )
{
	return !(s1 == s2);
}

#endif	// _CSTRING_H_


