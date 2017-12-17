
#ifndef	EXPRESSION_H
#define	EXPRESSION_H

#include	"cstring.h"
#include	"stdtypes.h"
#include	"Turtle.h"
#include	"math.h"

extern Ptr< Turtle >		turtle;

#define	DIFF	1e-10	// minimum float number

#define	D2R(x)	((x) * M_PI / 180)
#define	R2D(x)	((x) * 180 / M_PI)

class Constant : virtual public Reference {

    public:
	Constant( double value = 0.0 ) : _type(DOUBLE), _double(value) { }
	Constant( const String &name ) : _type(VARIABLE), _variable(name) { }

	operator double() const;
	const String	&variable_name() const;
	void		set( double value );
	void		set( String &variable );

    private:
	enum Type {
		DOUBLE, VARIABLE
	} _type;

	double	_double;
	String	_variable;
};

class Operator : virtual public Reference {

    public:
	virtual	~Operator() { }

	virtual	int		arity() const = 0;
	virtual	Constant	value( const Array< Constant > & ) const = 0;
};

class CallOperator : public Operator {

    public:
	CallOperator( const char *name, int arity );
		
	int	 arity() const;
	Constant value( const Array<Constant> &opers ) const;

    private:
	String	_name;
	int	_arity;
};

class PlusOperator : public Operator {

	int	arity() const
	{
		return 2;
	}
	
	Constant	value( const Array<Constant> &opers ) const
	{
		return opers[1] + opers[0];
	}
};

class MinusOperator : public Operator {

	int	arity() const
	{
		return 2;
	}
	
	Constant	value( const Array< Constant> &opers ) const
	{
		return opers[1] - opers[0];
	}
};

class MulOperator : public Operator {

	int	arity() const
	{
		return 2;
	}
	
	Constant	value( const Array< Constant> &opers ) const
	{
		return opers[1] * opers[0];
	}
};

class ModOperator : public Operator {

	int	arity() const
	{
		return 2;
	}
	
	Constant	value( const Array< Constant> &opers ) const
	{
		if( fabs(opers[0]) >= DIFF ) {
			return int(opers[1]+0.5) % int(opers[0]+0.5);
		} else {
			return 0;
		}
	}
};

class DivOperator : public Operator {

	int	arity() const
	{
		return 2;
	}
	
	Constant	value( const Array< Constant > &opers ) const
	{
		if( fabs(opers[0]) >= DIFF ) {
			return opers[1] / opers[0];
		} else {
			return 0;
		}
	}
};

class UnaryPlusOperator : public Operator {

	int	arity() const
	{
		return 1;
	}
	
	Constant	value( const Array< Constant> &opers ) const
	{
		return opers[0];
	}
};

class UnaryMinusOperator : public Operator {

	int	arity() const
	{
		return 1;
	}
	
	Constant	value( const Array< Constant> &opers ) const
	{
		return -opers[0];
	}
};

class EqualOperator : public Operator {

	int	arity() const
	{
		return 2;
	}

	Constant	value( const Array< Constant> &opers ) const;
};

class IsEqualOperator : public Operator {

	int	arity() const
	{
		return 2;
	}

	Constant	value( const Array< Constant> &opers ) const
	{
		return fabs(opers[0] - opers[1]) < DIFF;
	}
};

class LessEqualOperator : public Operator {

	int	arity() const
	{
		return 2;
	}

	Constant	value( const Array< Constant> &opers ) const
	{
		return fabs(opers[0]-opers[1]) < DIFF || opers[1] < opers[0];
	}
};

class LessOperator : public Operator {

	int	arity() const
	{
		return 2;
	}

	Constant	value( const Array< Constant> &opers ) const
	{
		return opers[1] < opers[0];
	}
};

class GreaterEqualOperator : public Operator {

	int	arity() const
	{
		return 2;
	}

	Constant	value( const Array< Constant> &opers ) const
	{
		return fabs(opers[0]-opers[1]) < DIFF || opers[1] > opers[0];
	}
};

class GreaterOperator : public Operator {

	int	arity() const
	{
		return 2;
	}

	Constant	value( const Array< Constant> &opers ) const
	{
		return opers[1] > opers[0];
	}
};

class AndOperator : public Operator {

	int	arity() const
	{
		return 2;
	}

	Constant	value( const Array< Constant> &opers ) const
	{
		return opers[1] > 0 && opers[0] > 0;
	}
};

class OrOperator : public Operator {

	int	arity() const
	{
		return 2;
	}

	Constant	value( const Array< Constant> &opers ) const
	{
		return opers[1] > 0 || opers[0] > 0;
	}
};

class PrintOperator : public Operator {

	int	arity() const
	{
		return 1;
	}

	Constant	value( const Array< Constant> &opers ) const
	{
		cout << "output: " << opers[0] << endl;
		return 0;
	}
};

class SinOperator : public Operator {

	int	arity() const
	{
		return 1;
	}

	Constant	value( const Array< Constant> &opers ) const
	{
		return sin(D2R(opers[0]));
	}
};

class CosOperator : public Operator {

	int	arity() const
	{
		return 1;
	}

	Constant	value( const Array< Constant> &opers ) const
	{
		return cos(D2R(opers[0]));
	}
};

class TanOperator : public Operator {

	int	arity() const
	{
		return 1;
	}

	Constant	value( const Array< Constant> &opers ) const
	{
		return tan(D2R(opers[0]));
	}
};

class ATanOperator : public Operator {

	int	arity() const
	{
		return 1;
	}

	Constant	value( const Array< Constant> &opers ) const
	{
		return R2D(atan(opers[0]));
	}
};

class ACosOperator : public Operator {

	int	arity() const
	{
		return 1;
	}

	Constant	value( const Array< Constant> &opers ) const
	{
		return R2D(atan(opers[0]));
	}
};

class ForwardOperator : public Operator {

	int	arity() const
	{
		return 1;
	}

	Constant	value( const Array< Constant> &opers ) const
	{
		turtle->forward( opers[0] );
		return 0;
	}
};

class BackwardOperator : public Operator {

	int	arity() const
	{
		return 1;
	}

	Constant	value( const Array< Constant> &opers ) const
	{
		turtle->backward( opers[0] );
		return 0;
	}
};

class LeftOperator : public Operator {

	int	arity() const
	{
		return 1;
	}

	Constant	value( const Array< Constant> &opers ) const
	{
		turtle->left( opers[0] );
		return 0;
	}
};

class RightOperator : public Operator {

	int	arity() const
	{
		return 1;
	}

	Constant	value( const Array< Constant> &opers ) const
	{
		turtle->right( opers[0] );
		return 0;
	}
};

class PenUpOperator : public Operator {

	int	arity() const
	{
		return 0;
	}

	Constant	value( const Array< Constant> & ) const
	{
		turtle->pen_up();
		return 0;
	}
};

class PenDownOperator : public Operator {

	int	arity() const
	{
		return 0;
	}

	Constant	value( const Array< Constant> & ) const
	{
		turtle->pen_down();
		return 0;
	}
};

class DirectionOperator : public Operator {

	int	arity() const
	{
		return 1;
	}

	Constant	value( const Array< Constant> &opers ) const
	{
		turtle->direction( opers[0] );
		return 0;
	}
};

class PositionOperator : public Operator {

	int	arity() const
	{
		return 2;
	}

	Constant	value( const Array< Constant> &opers ) const
	{
		turtle->position( opers[1], opers[0] );
		return 0;
	}
};

//
//	Expression
//

class Expression : virtual public Reference {

    public:
	class ILLEGAL_STACK : virtual public Reference { };

	Expression	&add( Ptr<Operator> );
	Expression	&add( const Constant & );
	Constant	evaluate() throw( ILLEGAL_STACK );

    private:

	struct Order : virtual public Reference {
	    public:
		enum Type {
			OPERATOR, OPERANDUS
		} _type;

		Order( Type type ) : _type(type) { }
		operator Type() { return _type; }
	};

	typedef RPtr<Operator>	RefOperator;
	Stack<Order>		_order;
	Stack<RefOperator>	_operators;
	Stack<Constant>		_operanduses;

	Constant	_evaluate( Ptr< Stack<Order> >,
				  Ptr< Stack<RefOperator> >,
				  Ptr< Stack<Constant> > )
		throw( ILLEGAL_STACK );
};

#endif /* EXPRESSION_H */
