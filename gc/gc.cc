
#include	<iostream.h>
#include	<fstream.h>

#include	"queue.h"

class Integer : public Reference {

    public:
	Integer( int number = 0 ) : number(number) 
	{
		cerr << "constructor: " << number << endl;
	}
	Integer( const Integer &i ) : number(i.number)
	{
		cerr << "constructor: " << number << endl;
	}
virtual	~Integer()
	{
		cerr << "destructor: " << number << endl;
	}

	operator int & () { return number; }

    private:
	int	number;
};

/*----------------------------------------------------------------------
 *	main
 *----------------------------------------------------------------------*/

int main()
{
	Queue< Integer > queue;

	cout << "Hello!\n";
	cin >> queue;
	queue.in( 6 );
	cout << queue.head() << endl;
	cout << queue << endl;
	cout << "out: " << queue.out();
	cout << ", " << queue.out() << endl;
	cout << queue << endl;
	cout << "this is the end...\n";
 
	return 0;
}
