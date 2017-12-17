
#include	"funcs.hh"

template< class ELEM >
List<ELEM>::List() : elements( 0 )
{
	head = new Node( ELEM(), NULL, NULL );
	head->prev = head;
	head->next = head;
}

template< class ELEM >
List<ELEM>::List( const List &list ) : elements( 0 )
{
	head = new Node( ELEM(), NULL, NULL );
	head->prev = head;
	head->next = head;

	copy( list.begin(), list.end(), back_insert_iterator<List>(*this) );
}

template< class ELEM >
List<ELEM>::~List()
{
	while( !is_empty() ) destroy( begin() );
}

template< class ELEM >
List<ELEM> &List<ELEM>::operator = ( const List &list )
{
	if( &list != this ) {
		while( !is_empty() ) destroy( begin() );
		copy( list.begin(), list.end(), 
		     back_insert_iterator<List>(*this) );
	}
	return *this;
}

template< class ELEM >
List<ELEM> &List<ELEM>::push_front( const ELEM &elem )
{
	return insert( elem, begin() );
}

template< class ELEM >
List<ELEM> &List<ELEM>::push_back( const ELEM &elem )
{
	return insert( elem, end() );
}

//----------------------------------------------------------------------
/*
int main()
{
	List<char *>	str;

	str.push_back( "first\n" );
	str.push_back( "end\n" );
	str.push_back( "garbage\n" );
	List<char *>	str2 = str;

	copy( str2.begin(), str2.end(), ostream_iterator<char *>(cout) );
	str2.destroy( str2.begin() );
	List<char *>	str3;

	str3 = str2;
	copy( str3.begin(), str3.end(), ostream_iterator<char *>(cout) );
	copy( str.begin(), str.end(), ostream_iterator<char *>(cout) );

	return 0;
}
*/
