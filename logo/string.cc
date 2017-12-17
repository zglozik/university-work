
#include	<string.h>

#include	"cstring.h"

String operator + ( const String &s1, const String &s2 )  {
	int	len;

	len = s1.len() + s2.len();
	String str(len);
	strcpy( str.data, s1.data );
	strcat( str.data, s2.data );
	str._len = len;

	return str;
}

bool operator < ( const String &s1, const String &s2 )  {

	return strcmp(s1.data, s2.data) < 0;
}

bool  operator == ( const String &s1, const String &s2 )  {

	return strcmp(s1.data, s2.data) == 0;
}

String::String( int size ) : _len(0), size(size+1) {

	data = new char [this->size];
	*data = '\0';
}

String::String( const char *s ) {

	_len = strlen(s);
	size = _len + 1;
	data = new char [size];
	strcpy( data, s );
}

String::String( const String &str ) {

	_len = str.len();
	size = _len + 1;
	data = new char [size];
	strcpy( data, str.data );
}

String::~String()  {

	delete [] data;
}

String &String::operator = ( const String &str )  {

	if( this != &str ) {
		if( size <= str.len() ) {
			char *tmp_data = new char [size = str.len()+1];
			delete [] data;
			data = tmp_data;
		}
		_len = str.len();
		strcpy( data, str.data );
	}
	return *this;
}

const char &String::operator []( int i ) const
{
	return data[i];
}

// ----------------------------------------------------------------------
/*
#include	<iostream.h>

int main()
{
	String	s1, s2( 20 ), s3( "3-ik" );

	cout << s1 << endl;
	s1 = "elso";
	cout << s1 << endl;
	s2 = s1;
	cout << s2 << endl;
	String s4 = s1 + s2 + s3;
	cout << s4 << endl;
	s1 = s4;
	cout << s1 << endl;
	
	return 0;
}
*/
