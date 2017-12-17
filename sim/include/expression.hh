
#ifndef	_EXPRESSION_HH_
#define	_EXPRESSION_HH_

#include	<stdlib.h>
#include	<math.h>
#include	"array.hh"
#include	"map.hh"
#include	"string.hh"

// ----------------------------------------------------------------------
//	SymbolTable
// ----------------------------------------------------------------------

typedef	Map<String, double>	SymbolTable;

// ----------------------------------------------------------------------
//	Expression
// ----------------------------------------------------------------------

class Expression {

//	opers

    public:
virtual	bool	evaluate( double &result ) = 0;

};

// ----------------------------------------------------------------------
//	Variable
// ----------------------------------------------------------------------

class Variable : public Expression {

    public:
	Variable( const SymbolTable::iterator &pos ) : pos(pos)	{ }

	bool evaluate( double &result )
	{
		result = *pos;
		return true;
	}

    private:
	SymbolTable::iterator	pos;
};

// ----------------------------------------------------------------------
//	Number
// ----------------------------------------------------------------------

class Number : public Expression {

    public:
	Number( double number ) : number(number) { }

	bool evaluate( double &result )
	{
		result = number; 
		return true;
	}

    private:
	double	number;
};

// ----------------------------------------------------------------------
//	BinaryExpression
// ----------------------------------------------------------------------

struct BinaryExpression : public Expression {

    private:
	BinaryExpression( const BinaryExpression & );
	BinaryExpression &operator = ( const BinaryExpression & );

    public:
	BinaryExpression( Expression *first, Expression *second ) 
	: first(first), second(second)
	{
		TEST_EXPR( first != NULL && second != NULL );
	}
virtual	~BinaryExpression() 
	{
		delete first;
		delete second;
	}

	bool evaluate( double &result )
	{
		double	p1, p2;
		bool	e1, e2;
		bool	er;
		e1 = first->evaluate( p1 );
		e2 = second->evaluate( p2 );
		if( e1 && e2 ) {
			er = evaluate( p1, p2, result );
		}
		return e1 && e2 && er;
	}

virtual	bool evaluate( double p1, double p2, double &result ) = 0;

    private:
	Expression	*first, *second;
};

// ----------------------------------------------------------------------
//	UnaryExpression
// ----------------------------------------------------------------------

struct UnaryExpression : public Expression {

    private:
	UnaryExpression( const UnaryExpression & );
	UnaryExpression &operator = ( const UnaryExpression & );

    public:
	UnaryExpression( Expression *first ) : first(first)
	{
		TEST_EXPR( first != NULL );
	}
virtual	~UnaryExpression() 
	{
		delete first;
	}

	bool evaluate( double &result )
	{
		double	p1;
		bool	e1;
		bool	er;
		e1 = first->evaluate( p1 );
		if( e1 ) {
			er = evaluate( p1, result );
		}
		return e1 && er;
	}

virtual	bool evaluate( double p1, double &result ) = 0;

    private:
	Expression	*first;
};


// ----------------------------------------------------------------------
//	Binary operators
// ----------------------------------------------------------------------

struct Addition : public BinaryExpression {

	Addition( Expression *first, Expression *second ) 
		: BinaryExpression( first, second ) { }

	bool evaluate( double p1, double p2, double &result )
	{
		result = p1 + p2;
		return true;
	}
};

struct Subtraction : public BinaryExpression {

	Subtraction( Expression *first, Expression *second ) 
		: BinaryExpression( first, second ) { }

	bool evaluate( double p1, double p2, double &result )
	{
		result = p1 - p2;
		return true;
	}
};

struct Division : public BinaryExpression {

	Division( Expression *first, Expression *second ) 
		: BinaryExpression( first, second ) { }

	bool evaluate( double p1, double p2, double &result )
	{
		if( p2 != 0.0 ) {
			result = p1 / p2;
			return true;
		}
		return false;
	}
};

struct Multiplication : public BinaryExpression {

	Multiplication( Expression *first, Expression *second ) 
		: BinaryExpression( first, second ) { }

	bool evaluate( double p1, double p2, double &result )
	{
		result = p1 * p2;
		return true;
	}
};

struct Power : public BinaryExpression {

	Power( Expression *first, Expression *second ) 
		: BinaryExpression( first, second ) { }

	bool evaluate( double p1, double p2, double &result )
	{
		errno = 0;
		result = pow( p1, p2 );
		return !errno;
	}
};

// ----------------------------------------------------------------------
//	Unary operators
// ----------------------------------------------------------------------

struct UnaryPlus : public UnaryExpression {

	UnaryPlus( Expression *first ) : UnaryExpression(first) { }

	bool evaluate( double p1, double &result )
	{
		result = p1;
		return true;
	}
};

struct UnaryMinus : public UnaryExpression {

	UnaryMinus( Expression *first ) : UnaryExpression(first) { }

	bool evaluate( double p1, double &result )
	{
		result = -p1;
		return true;
	}
};

struct Exp : public UnaryExpression {

	Exp( Expression *first ) : UnaryExpression(first) { }

	bool evaluate( double p1, double &result )
	{
		result = exp( p1 );
		return true;
	}
};

struct Tangent : public UnaryExpression {

	Tangent( Expression *first ) : UnaryExpression(first) { }

	bool evaluate( double p1, double &result )
	{
		errno = 0;
		result = tan( p1 );
		return !errno;
	}
};

struct Cosinus : public UnaryExpression {

	Cosinus( Expression *first ) : UnaryExpression(first) { }

	bool evaluate( double p1, double &result )
	{
		result = cos( p1 );
		return true;
	}
};

struct Sinus : public UnaryExpression {

	Sinus( Expression *first ) : UnaryExpression(first) { }

	bool evaluate( double p1, double &result )
	{
		result = sin( p1 );
		return true;
	}
};

struct Sqrt : public UnaryExpression {

	Sqrt( Expression *first ) : UnaryExpression(first) { }

	bool evaluate( double p1, double &result )
	{
		errno = 0;
		result = sqrt( p1 );
		return !errno;
	}
};

#endif // _EXPRESSION_HH_
