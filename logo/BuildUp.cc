
#include	"BuildUp.h"
#include	"SymbolTable.h"

extern Ptr< ProcSymbolTable >	proc_symbol_table;

//
//	Command class
//

void Command::expression( Ptr< Expression > expr )
{
	_expr = expr;
}

void Command::execute()
{
	_expr->evaluate();
}

//
//	Sequence class
//

void Sequence::add( Ptr< BuildUp > program )
{
	_program.append( program );
}

void Sequence::execute()
{
	for( int i = 0; i < _program.num_elements(); i++ )
		_program[i]->execute();
}

//
//	While class
//

void While::condition( Ptr< Expression > condition )
{
	_condition = condition;
}

void While::body( Ptr< Sequence > sequence )
{
	_sequence = sequence;
}

void While::execute()
{
	while( _condition->evaluate() > 0.0 ) {
		_sequence->execute();
	}
}

//
//	If class
//

void If::condition( Ptr< Expression > condition )
{
	_condition = condition;
}

void If::then_branch( Ptr< Sequence > sequence )
{
	_then_branch = sequence;
}

void If::else_branch( Ptr< Sequence > sequence )
{
	_else_branch = sequence;
}

void If::execute()
{
	if( _condition->evaluate() > 0.0 ) {
		_then_branch->execute();
	} else if( _else_branch ) {
		_else_branch->execute();
	}
}

