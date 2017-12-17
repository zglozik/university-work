
#include	<string.h>
#include	<iostream.h>

class Test {

    private:
	char	*_name;
	static int	count;
	int	mynumber;

    public:

	Test( const char *name );
	Test( const Test &test );

	virtual ~Test();
};

int Test::count = 0;

Test::Test( const char *name )
{
	cout << "constructor: " << name << ", " << ++count << '\n';
	_name = strdup( name );
	mynumber = count;
}

Test::Test( const Test &test )
{
	cout << "copy constuctor: " << test._name << ", " << ++count
		<< '\n';
	_name = strdup( test._name );
	mynumber = count;
}

Test::~Test()
{
	cout << "destructor: " << _name << ", " << mynumber << '\n';
	delete [] _name;
}

void f()
{
	cout << "function: f\n";
	throw Test( "exception from f" );
	cout << "function end\n";
}

void main()
{
	Test	t1( "t1" );

	try {
		Test	t( "t2" );

		f();
	}
	catch( const Test error ) {
		cout << "exception handler\n";
	}
}
