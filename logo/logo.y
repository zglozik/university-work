
%{

#include	<stdio.h>
#include	"logo.h"

extern int	yylex();
extern int	line_no;

#define	MAX_ID_LEN	30

%}

%union{
	char		id[MAX_ID_LEN+1];
	double		number;
	Array<String>	*par_names;
	int		par_num;
}

%token	<number>	NUMBER
%token	<id>		WHILE, IF, THEN, ELSE, FUNCTION, END
%token	<id>		ID, SIN, COS, TAN, ACOS, ATAN
%token	<id>		FORWARD, BACKWARD, LEFT, RIGHT, PEN, UP, PEN, DOWN
%token	<id>		PRINT, DIRECTION, POSITION
%left	'&'
%left	'|'
%left	'<', '>', '=', LE, GE
%left	'+', '-'
%left	'*', '/', '%'
%left	UNARYMINUS
%left	UNARYPLUS

%type	<par_names>	fparameters, fparameters2
%type	<par_num>	cparameters, cparameters2
%type	<id>		function_head

%%

S:	init command_list;

init:	{
		expr = new Expression();
		block = new Stack< Ptr<Sequence> >();
		while_stack = new Stack< Ptr<While> >();
		if_stack = new Stack< Ptr<If> >();
		block->push( new Sequence() );
	}

/*------------------------------------------------------------
 *	commands
 *------------------------------------------------------------*/

command_list:	
	|	'\n' command_list
	|	command '\n' command_list
	|	function_def '\n' command_list
	;

command:	simple_command
		{
			block->top()->add( new Command(expr) );
			expr = new Expression();
		}
	|	WHILE while_expression '\n' command_list END
		{
			while_stack->top()->body( block->pop() );
			block->top()->add( (While *) while_stack->pop() );
		}
	|	IF if_expression then_branch else_branch END
		{
			block->top()->add( (If *) if_stack->pop() );
		}
	;

function_def:	FUNCTION function_head '\n' 
		init_function command_list END
		{
			try {
				proc_symbol_table->define( $2, block->pop() );
			} 
			catch( const ProcSymbolTable::DEFINED & ) {
				char	buf[100];
				sprintf( buf, "function %s is already defined",
					$2 );
				yyerror( buf );
				YYERROR;
			}
		}
	;

/*------------------------------------------------------------
 *	simple command
 *------------------------------------------------------------*/

simple_command:		
		function_call
	|	assignment
	|	turtle_command
	;

/*------------------------------------------------------------
 *	while
 *------------------------------------------------------------*/

while_expression:	expression
			{
				while_stack->push( new While() );
				block->push( new Sequence() );

				while_stack->top()->condition( expr );
				expr = new Expression();
			}
	;

/*------------------------------------------------------------
 *	if
 *------------------------------------------------------------*/

if_expression:	expression
		{
			if_stack->push( new If() );
			if_stack->top()->condition( expr );
			expr = new Expression();
		}
	;

then_branch:	init_if THEN '\n' command_list
		{
			if_stack->top()->then_branch( block->pop() );
		}
	;

else_branch:
	|	init_if ELSE '\n' command_list
		{
			if_stack->top()->else_branch( block->pop() );
		}
	;

init_if:	{ block->push( new Sequence() ); }
	;

/*------------------------------------------------------------
 *	function
 *------------------------------------------------------------*/

function_head:	ID '(' fparameters ')'
		{
			proc_symbol_table->declare( $1, *$3 );
			delete $3;
			$$ = $1;
		}

fparameters:	{
			$$ = new Array<String>;
		}
	|	fparameters2
		{
			$$ = $1;
		}
	;

fparameters2:	ID
		{
			$$ = new Array<String>;
			$$->append( $1 );
		}
	|	ID ',' fparameters2
		{
			$$ = $3;
			$$->append( $1 );
		}
	;
		
init_function:	{ block->push( new Sequence() ); }
	;

/*------------------------------------------------------------
 *	assignment
 *------------------------------------------------------------*/

assignment:	variable ':' '=' expression
		{
			 expr->add( new EqualOperator() );
		}
	;

variable:	ID
		{
			expr->add( Constant($1) );
		}
	;

/*------------------------------------------------------------
 *	expression
 *------------------------------------------------------------*/

expression:	NUMBER		{ expr->add( Constant($1) ); }
	|	variable

	|	expression '+' expression
				{ expr->add( new PlusOperator() ); }
	|	expression '-' expression
				{ expr->add( new MinusOperator() ); }
	|	expression '*' expression
				{ expr->add( new MulOperator() ); }
	|	expression '/' expression
				{ expr->add( new DivOperator() ); }
	|	expression '%' expression
				{ expr->add( new ModOperator() ); }
	|	expression '=' expression
				{ expr->add( new IsEqualOperator() ); }
	|	expression '<' expression
				{ expr->add( new LessOperator() ); }
	|	expression LE expression
				{ expr->add( new LessEqualOperator() ); }
	|	expression '>' expression
				{ expr->add( new GreaterOperator() ); }
	|	expression GE expression
				{ expr->add( new GreaterEqualOperator() ); }
	|	expression '&' expression
				{ expr->add( new AndOperator() ); }
	|	expression '|' expression
				{ expr->add( new OrOperator() ); }
	|	'(' expression ')'

	|	'-' expression %prec UNARYMINUS
				{ expr->add( new UnaryMinusOperator() ); }
	|	'+' expression %prec UNARYPLUS
				{ expr->add( new UnaryPlusOperator() ); }
	|	function_call
	;

/*------------------------------------------------------------
 *	function call
 *------------------------------------------------------------*/

function_call:	ID '(' cparameters ')'
		{	
			Array<String>	params;
			Ptr<Sequence>	body;

			try {
				proc_symbol_table->get( $1, params, body );
			}
			catch( const ProcSymbolTable::UNDEFINED & ) {
				char	buf[100];
				sprintf(buf, "function %s is not defined", $1);
				yyerror( buf );
				YYERROR;
			}
			if( params.num_elements() != $3 ) {
				char	buf[100];
				sprintf( buf, "wrong number of parameters "
					"for %s", $1 );
				yyerror( buf );
				YYERROR;
			}
			expr->add( new CallOperator($1, $3) );
		}
	|	SIN '(' expression ')'
		{
			expr->add( new SinOperator() );
		}
	|	COS '(' expression ')'
		{
			expr->add( new CosOperator() );
		}
	|	TAN '(' expression ')'
		{
			expr->add( new TanOperator() );
		}
	|	ACOS '(' expression ')'
		{
			expr->add( new ACosOperator() );
		}
	|	ATAN '(' expression ')'
		{
			expr->add( new ATanOperator() );
		}

cparameters:
		{
			$$ = 0;
		}
	|	cparameters2
		{
			$$ = $1;
		}
	;

cparameters2:	expression
		{
			$$ = 1;
		}
	|	cparameters2 ',' expression
		{
			$$ = $1 + 1;
		}
	;
			
/*------------------------------------------------------------
 *	turtle command
 *------------------------------------------------------------*/

turtle_command:	PRINT expression
				{ expr->add( new PrintOperator() ); }
	|	FORWARD expression
				{ expr->add( new ForwardOperator() ); }
	|	BACKWARD expression
				{ expr->add( new BackwardOperator() ); }
	|	LEFT expression
				{ expr->add( new LeftOperator() ); }
	|	RIGHT expression
				{ expr->add( new RightOperator() ); }
	|	PEN UP
				{ expr->add( new PenUpOperator() ); }	
	|	PEN DOWN
				{ expr->add( new PenDownOperator() ); }
	|	DIRECTION expression
				{ expr->add( new DirectionOperator() ); }
	|	POSITION expression ',' expression
				{ expr->add( new PositionOperator() ); }
	;

%%

void yyerror( char *s )
{
	fprintf( stderr, "line %d: %s\n", line_no, s );
}
