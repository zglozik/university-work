
#include	"expression.hh"

// ----------------------------------------------------------------------
/*
int main()
{
	Expression	*expr;
	SymbolTable	st;
	double		result;
	bool		ok;

	st[ "pi" ] = M_PI;
	st[ "e" ] = exp(1);
	st[ "t" ] = 1;

	expr = new Multiplication( new Variable(st.find("pi")),
				  new Number(2) );
	ok = expr->evaluate( result );
	cout << result << ", " << ok << endl;
	delete expr;

	expr = new Subtraction( new Variable(st.find("e")),
				  new Number(1) );
	ok = expr->evaluate( result );
	cout << result << ", " << ok << endl;
	delete expr;

	expr = new Cosinus( new Variable(st.find("pi")) );
	ok = expr->evaluate( result );
	cout << result << ", " << ok << endl;
	delete expr;

	return 0;
}
*/
