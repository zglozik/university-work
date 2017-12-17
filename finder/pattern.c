
#include	<sys/types.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<regex.h>
#include	"finder.h"
#include	"pattern.h"

/*----------------------------------------------------------------------
 *	private types
 *----------------------------------------------------------------------*/

#define	STACK_SIZE	100

#define	MAKE_OP( op, pr )	(((pr) << 8) + op)
#define	PRIORITY( op )		((op) >> 8)
#define	OPERATOR( op )		((op) & 255)
#define	ARITY( op )		(OPERATOR(op) == '!' || OPERATOR(op) == '(' ? \
				 1 : 2)

#define	isop( op )	((op)=='&' || (op)=='|' || (op)=='!' ||	\
			 (op)=='(' || (op) == ')')

typedef enum {
	ARG, OP, END
} TokenType;

typedef enum {
	OPERANDUS, OPERATOR, ACCEPT, ERROR
} States;

typedef struct {
	TokenType	type;
	union {
		regex_t	regex;
		int	op;
	} data;
} StackType;

struct _Pattern {
	StackType	stack[STACK_SIZE];
	int		n_stack;		/* no. of elements in stack */
};

/*----------------------------------------------------------------------
 *	private functions
 *----------------------------------------------------------------------*/

static int operandus( Pattern pattern, StackType *token, char *token_str,
		     int stack[], int *n_stack, bool *consumed );
static int operator( Pattern pattern, StackType *token, char *token_str,
		    int stack[], int *n_stack, bool *consumed );
static bool pop_to_weaker_op( Pattern pattern, int stack[], int *n_stack,
			     int priority );
static bool pop_to_left_parenthesis( Pattern pattern, 
				    int stack[], int *n_stack );
static int push_operator( Pattern pattern, int op );
static char *read_token( const char **regex_str, StackType *token );
static bool regex_compile( regex_t *regex, const char *str );
static bool evaluate( StackType stack[], int *n_stack, const char *buffer );

/*----------------------------------------------------------------------
 *	Pattern pattern_create( const char *regex_str )
 *----------------------------------------------------------------------*/

Pattern pattern_create( const char *regex_str )
{
	Pattern pattern = (Pattern) malloc( sizeof(struct _Pattern) );
	int	stack[STACK_SIZE];
	int	n_stack = 0;
	char	*token_str;
	bool	consumed;
	StackType	token;
	States		state;

	if( !pattern ) return NULL;

	pattern->n_stack = 0;

	state = OPERANDUS;
	if( !(token_str=read_token(&regex_str, &token)) ) state = ERROR;
	while( state != ACCEPT && state != ERROR ) {
		switch( state ) {
		    case OPERANDUS:
			state = operandus( pattern, &token, token_str,
					  stack, &n_stack, &consumed );
			break;
		    case OPERATOR:
			state = operator( pattern, &token, token_str,
					 stack, &n_stack, &consumed );
			break;
		    default:
			/* it can't happen */
			assert( 1 );
		}
		if( consumed && !(token_str=read_token(&regex_str, &token)) )
			state = ERROR;
	}
	if( state == ERROR ) {
		free( pattern );
		pattern = NULL;
	}
	return pattern;
}

/*----------------------------------------------------------------------
 *	int operandus( Pattern pattern, StackType *token, char *token_str, 
 *			int stack[], int *n_stack, bool *consumed )
 *----------------------------------------------------------------------*/

static int operandus( Pattern pattern, StackType *token, char *token_str, 
		     int stack[], int *n_stack, bool *consumed )
{
	int state;
	*consumed = true;

	switch( token->type ) {
	    case OP:
		if( ARITY(token->data.op) == 1 && 
		   OPERATOR(token->data.op) != ')' ) {
			stack[(*n_stack)++] = token->data.op;
			state = OPERANDUS;
		} else { /* arity == 2 or ')' */
			state = ERROR;
			error( "unexpected operator: %c",
			      OPERATOR(token->data.op) );
		}
		break;
	    case ARG:
		/* printf( "operandus:\t%s\n", token_str ); */

		pattern->stack[pattern->n_stack++] = *token;
		state = OPERATOR;
		break;
	    case END:
		state = ACCEPT;
		break;
	}
	return state;
}


/*----------------------------------------------------------------------
 *	int operator( Pattern pattern, StackType *token, char *token_str, 
 *			int stack[], int *n_stack, bool *consumed )
 *----------------------------------------------------------------------*/

static int operator( Pattern pattern, StackType *token, char *token_str, 
		    int stack[], int *n_stack, bool *consumed )
{
	States		state;
	StackType	ttoken;

	*consumed = true;
	switch( token->type ) {
	    case ARG:
	    case OP:
		if( token->type == ARG || ARITY(token->data.op) == 1 ) {
			ttoken.type = OP;
			ttoken.data.op = MAKE_OP( '&', 10 );	/* default */
			token = &ttoken;
			*consumed = false;
		}
		if( OPERATOR(token->data.op) == ')' ) {
			if( pop_to_left_parenthesis(pattern,stack,n_stack) ) {
				state = OPERATOR;
			} else {
				state = ERROR;
				error( "missing left parenthesis" );
			}
		} else if( ARITY(token->data.op) == 2 ) {
			pop_to_weaker_op( pattern, stack, n_stack,
					 PRIORITY(token->data.op) );
			stack[(*n_stack)++] = token->data.op;
			state = OPERANDUS;
		}
		break;
	    case END:
		if( pop_to_left_parenthesis(pattern, stack, n_stack) ) {
			state = ERROR;
			error( "missing right parenthesis" );
		} else {
			state = OPERANDUS;
		}
		break;
			
	}
	return state;
}

/*----------------------------------------------------------------------
 *	bool pop_to_weaker_op( Pattern pattern, int stack[], int *n_stack,
 *				int priority )
 *----------------------------------------------------------------------*/

static bool pop_to_weaker_op( Pattern pattern, int stack[], int *n_stack,
			     int priority )
{
	bool found = false;
	while( *n_stack > 0 && !found ) {
		found = PRIORITY(stack[*n_stack-1]) < priority;
		if( !found ) {
			push_operator( pattern, stack[--*n_stack] );
		}
	}
	
	return found;
}

/*----------------------------------------------------------------------
 *	bool pop_to_left_parenthesis( Pattern pattern, 
 *				int stack[], int *n_stack )
 *----------------------------------------------------------------------*/

static bool pop_to_left_parenthesis( Pattern pattern, 
				    int stack[], int *n_stack )
{
	bool found;
	for( found = false; *n_stack > 0 && !found; --*n_stack ) {
		found = OPERATOR(stack[*n_stack-1]) == '(';
		if( !found ) {
			push_operator( pattern, stack[*n_stack-1] );
		}
	}
	return found;
}
	
/*----------------------------------------------------------------------
 *	int push_operator( Pattern pattern, int op )
 *----------------------------------------------------------------------*/

static int push_operator( Pattern pattern, int op )
{
	pattern->stack[pattern->n_stack].type = OP;
	pattern->stack[pattern->n_stack].data.op = op;
	/* printf( "operator:\t%c\n", op ); */

	return ++pattern->n_stack;
}

/*----------------------------------------------------------------------
 *	char *read_token( const char **regex_str, StackType *token )
 *----------------------------------------------------------------------*/

static char *read_token( const char **regex_str, StackType *token )
{
	static char	buffer[100];
	char	*pbuffer = buffer;
	bool	rv = true;

	while( isspace(**regex_str) ) ++*regex_str;

	switch( **regex_str ) {
	    case '\0':
		token->type = END;
		break;
	    case '&':
		token->type = OP;
		token->data.op = MAKE_OP( **regex_str, 10 );
		++*regex_str;
		break;
	    case '|':
		token->type = OP;
		token->data.op = MAKE_OP( **regex_str, 5 );
		++*regex_str;
		break;
	    case '!':
		token->type = OP;
		token->data.op = MAKE_OP( **regex_str, 15 );
		++*regex_str;
		break;
	    case '(':
		token->type = OP;
		token->data.op = MAKE_OP( **regex_str, 0 );
		++*regex_str;
		break;
	    case ')':
		token->type = OP;
		token->data.op = MAKE_OP( **regex_str, 0 );
		++*regex_str;
		break;
	    default:
		token->type = ARG;
		while( **regex_str && !isspace(**regex_str) && 
		      !isop(**regex_str) ) {
			if( **regex_str == '\\' ) {
				if( *++*regex_str ) 
					*pbuffer++ = *(*regex_str)++;
			} else {
				*pbuffer++ = *(*regex_str)++;
			}
		}
		*pbuffer = '\0';

		rv = regex_compile( &token->data.regex, buffer );
	}
	return rv ? buffer : NULL;
}

/*----------------------------------------------------------------------
 *	bool regex_compile( regex_t *regex, const char *str )
 *----------------------------------------------------------------------*/

static bool regex_compile( regex_t *regex, const char *str )
{
	int	err;
	if( (err = regcomp(regex, str, REG_ICASE | REG_NOSUB | REG_EXTENDED)) 
		!= 0) {
		char	errstr[100];

		regerror( err, regex, errstr, 100 );
		error( "can't compile regex: %s, %s", str, errstr );
		return false;
	}
	return true;
}

/*----------------------------------------------------------------------
 *	void pattern_free( Pattern pattern )
 *----------------------------------------------------------------------*/

void pattern_free( Pattern pattern )
{
	free( pattern );
}

/*----------------------------------------------------------------------
 *	bool pattern_match( Pattern pattern, const char *str )
 *----------------------------------------------------------------------*/

bool pattern_match( Pattern pattern, const char *str )
{
	int	n_stack = pattern->n_stack;
	bool	found;
	
	found = evaluate( pattern->stack, &n_stack, str );

	return found;
}

/*----------------------------------------------------------------------
 *	bool evaluate( StackType stack[], int *n_stack, const char *buffer )
 *----------------------------------------------------------------------*/

static bool evaluate( StackType stack[], int *n_stack, const char *buffer )
{
	bool	rv;
	if( *n_stack == 0 ) {
		rv = false;
	} else if( stack[*n_stack-1].type == OP ) {
		int	op = stack[--*n_stack].data.op;
		bool	rv2 = evaluate( stack, n_stack, buffer );
		switch( OPERATOR(op) ) {
		    case '|':
			rv = rv2 || evaluate( stack, n_stack, buffer );
			break;
		    case '&':
			rv = rv2 && evaluate( stack, n_stack, buffer );
			break;
		    case '!':
			rv = !rv2;
			break;
		    default:
			/* this can't be */
			assert( 1 );
		}
	} else {	/* ARG */
		rv = !regexec( &stack[--*n_stack].data.regex, buffer,
			      0, NULL, REG_NOTBOL | REG_NOTEOL );
	}
	return rv;
}

/*----------------------------------------------------------------------
 *	test module
 *----------------------------------------------------------------------*/
/*
int main( int argc, char *argv[] )
{
	Pattern pattern;

	prog_name = argv[0];
	pattern = pattern_create( argv[1] );
	pattern_free( pattern );

	return 0;
}
*/
