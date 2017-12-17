
#include	"Expression.h"
#include	"SymbolTable.h"

extern Ptr< SymbolTable >	symbol_table;
extern Ptr< ProcSymbolTable >	proc_symbol_table;

Constant EqualOperator::value( const Array< Constant > &opers ) const
{
	symbol_table->set( opers[1].variable_name(), opers[0] );
	return opers[0];
}

CallOperator::CallOperator( const char *name, int arity )
{
	_arity = arity;
	_name = name;
}

int CallOperator::arity() const
{
	return _arity;
}

Constant CallOperator::value( const Array<Constant> &opers ) const
{
	Array<String>	params;
	Ptr<Sequence>	body;

	proc_symbol_table->get( _name, params, body );
	assert( _arity == params.num_elements() );

	symbol_table->begin();
	for( int i = 0; i < params.num_elements(); i++ ) {
		symbol_table->append( params[i], opers[i] );
	}
	symbol_table->append( "ret", 0 );

	body->execute();

	double rv = symbol_table->get( "ret" );
	symbol_table->end();

	return rv;
}

//
//	Constant class
//

Constant::operator double() const
{
	double	value;
	
	switch( _type ) {
	    case DOUBLE:
		value = _double;
		break;
	    case VARIABLE:
		try {
			value = symbol_table->get( _variable );
		} 
		catch( const SymbolTable::UNDEFINED & ) {
			value = 0;
		}
		break;
	}
	return value;
}

const String &Constant::variable_name() const
{
	assert( _type == VARIABLE );

	return _variable;
}

void Constant::set( double value )
{
	_type = DOUBLE;
	_double = value;
}

void Constant::set( String &variable )
{
	_type = VARIABLE;
	_variable = variable;
}

//
//	Expression class
//

Expression &Expression::add( Ptr<Operator> or )
{
	_operators.push( or );
	_order.push( Order::OPERATOR );
	return *this;
}

Expression &Expression::add( const Constant &os )
{
	_operanduses.push( os );
	_order.push( Order::OPERANDUS );
	return *this;
}

Constant Expression::evaluate() throw( ILLEGAL_STACK )
{
	Ptr< Stack<Order> > torder = new Stack<Order>( _order );
	Ptr< Stack<RefOperator> > toperators =
		new Stack<RefOperator>( _operators );
	Ptr< Stack<Constant> > toperanduses =
		new Stack<Constant>( _operanduses );
	return _evaluate( torder, toperators, toperanduses );
}


Constant Expression::_evaluate(	Ptr< Stack<Order> > order, 
			       Ptr< Stack<RefOperator> > operators,
			       Ptr< Stack<Constant> > operanduses ) 
	throw( ILLEGAL_STACK )
{
	if( order->is_empty() ) throw ILLEGAL_STACK();

	Order current_order = order->pop();
	Constant 		result;
	Ptr< Operator >		top_operator;
	Array< Constant >	opers;

	switch( current_order ) {
	    case Order::OPERATOR:
		top_operator = operators->pop();

		for( int i = top_operator->arity()-1; i >= 0; i-- ) 
			opers.append( _evaluate(order,operators,operanduses) );
		result = top_operator->value( opers );
		break;
	    case Order::OPERANDUS:
		result = operanduses->pop();
		break;
	}
	return result;
}
