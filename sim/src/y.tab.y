
%{

#include	"bodyspec.hh"
#include	<string.h>

extern int yylex(void);

int	yyline_no;	// to count input lines

// private variables

static Map<String,WristMotion*>	*wrist_motions = 0;
static Body			*body = 0;

static BodyBuilder	*body_builder = 0;
static String		current_wrist;
static List<String>	*wrist_names = 0;
static bool		first_boundary;
static double		last_point;
static Point		speed; // start speed

static List<Expression *>	*litter = 0;

// private functions

static void gc_push( Expression *result, Expression *p1=0, Expression *p2=0 );
static bool init_wrists();
static bool check_bound( double last, double next );
static bool add_section( const Boundaries &b, Expression *e );
static bool make_point( Expression *p1, Expression *p2, Expression *p3, 
		       _Point &p );
static bool evaluate( Expression *e, double &result );
static inline SymbolTable &symbol_table();
static void yyerror( const char *s );

%}

%union {
	Expression	*expr;
	Boundaries	boundaries;
	_Point		point;
	PartPoint	part_point;
	double		number;
	bool		route;
	char		id[MAX_ID_LEN+1];
}

%token	BODY
%token	PARTS
%token	WRISTS
%token	INIT
%token	PROGRAM
%token	FIXED
%token	START
%token	END

%left	SQRT
%left	SIN
%left	COS
%left	TAN
%left	EXP

%token	<number>	NUMBER
%token	<id>		ID
%token	<route>		ROUTE

%type	<expr>		expression, expr, section_program
%type	<boundaries>	boundaries
%type	<id>		wrist_id
%type	<point>		point
%type	<part_point>	part_point

%left	'+', '-'
%left	'*', '/'
%right	'^'
%left	UNARYPLUS
%left	UNARYMINUS

%%

/*----------------------------------------------------------------------
 *	s
 *----------------------------------------------------------------------*/

s:
	BODY PARTS ROUTE body_parts 
	WRISTS wrist_parts 
	PROGRAM program_parts
	INIT init_part
	{
		TEST_EXPR( body_builder != NULL & wrist_motions != NULL );

		body = (*body_builder)( $3 );
		delete body_builder;
		body_builder = NULL;
		if( !body ) {
			yyerror( "I can't create body." );
			YYERROR;
		}
	}
	;

/*----------------------------------------------------------------------
 *	init_part
 *----------------------------------------------------------------------*/

init_part:
	point part_point ':' point ',' expression
	{
		speed = Point($1.x, $1.y, $1.z);
		bool ok = init_wrists();
		if( ok ) {
			double angle;
			ok = evaluate( $6, angle );
			if( ok ) {
				ok = body_builder->init( $2.id, $2.endpoint,
					Point($4.x, $4.y, $4.z), angle );
			}
			if( !ok ) yyerror( body_builder->error() );
		}
		delete $6;
		if( !ok ) YYERROR;
	}
	;

/*----------------------------------------------------------------------
 *	program_parts
 *----------------------------------------------------------------------*/

program_parts:
	program
	| program_parts program
	;

program:
	wrist_id ':' begin_sections sections

wrist_id:
	ID
	{
		List<String>::iterator pos = 
			find( wrist_names->begin(), wrist_names->end(),
				String($1) );
		if( pos == wrist_names->end() ) {
			char buf[200];
			sprintf( buf, "Wrist %s doesn't exist.", $1 );
			yyerror( buf );
			YYERROR;
		}
		Map<String,WristMotion*>::iterator pwm = 
			wrist_motions->find( $1 );
		if( pwm != wrist_motions->end() ) {
			char buf[200];
			sprintf(buf,"Motion of wrist %s is already defined.",
				$1 );
			yyerror( buf );
			YYERROR;
		}
		current_wrist = $1;
		WristMotion *wm = new WristMotion;
		wm->start( 0 );
		wm->symbol_table()[ "t" ] = 0;
		(*wrist_motions)[ current_wrist ] = wm;

		strcpy( $$, $1 );
	}
	;

begin_sections:	{ first_boundary = true; last_point = 0; }

sections:
	section
	| section sections
	;

section:
	boundaries ':' section_program ';'
	{
		bool ok = add_section( $1, $3 );
		if( !ok ) YYERROR;
	}
	;

boundaries:
	NUMBER
	{
		bool ok;
		if( first_boundary && $1 != 0 ) {
			yyerror("Section must start with 0 parameter value.");
			ok = false;
		} else {
			ok = check_bound( last_point, $1 );
		}
		if( !ok ) YYERROR;

		$$.size = 1;
		$$.bounds[0] = $1;
		first_boundary = false;
		last_point = $1;
	}
	| '-' NUMBER
	{
		if( check_bound( last_point, $2 ) ) {
			$$.size = 2;
			$$.bounds[0] = last_point;
			$$.bounds[1] = $2;
		} else {
			YYERROR;
		}
		first_boundary = false;
		last_point = $2;
	}
	| NUMBER '-' NUMBER
	{
		bool ok = true;
		if( first_boundary && $1 != 0 ) {
			yyerror("Section must start with 0 parameter value.");
		}
		if( check_bound(last_point, $1) && check_bound($1,$3) ) {
			$$.size = 2;
			$$.bounds[0] = $1;
			$$.bounds[1] = $3;
		} else {
			ok = false;
		}
		if( !ok ) YYERROR;

		first_boundary = false;
		last_point = $3;
	}
	;

section_program:
	FIXED { $$ = 0; }
	| expression { TEST_EXPR( $1 != NULL ); $$ = $1; }
	;

/*----------------------------------------------------------------------
 *	wrist_parts
 *----------------------------------------------------------------------*/

wrist_parts:
	wrist
	| wrist_parts wrist
	;

wrist:
	ID ':' part_point ',' part_point ',' point
	{
		bool ok = body_builder->add_wrist( $1,
				$3.id, $3.endpoint, $5.id, $5.endpoint,
				Point( $7.x, $7.y, $7.z ) );
		if( !ok ) {
			yyerror( body_builder->error() );
			YYERROR;
		}
		wrist_names->push_back( $1 );
	}
	;

/*----------------------------------------------------------------------
 *	body_parts
 *----------------------------------------------------------------------*/

body_parts:
	part
	| part body_parts
	;

part:
	ID ':' 
		expression',' /* length */
		expression',' /* weight */
		expression',' /* centre */
		ROUTE	      /* shall we draw route? */
	{
		double	l, w, c;
		bool ok = evaluate( $3, l );
		delete $3;
		ok = ok && evaluate( $5, w );
		delete $5;
		ok = ok && evaluate( $7, c );
		delete $7;

		if( ok ) {
			ok = body_builder->add_part( $1, l, w, c, $9 );
			if( !ok ) yyerror( body_builder->error() );
		}
		if( !ok ) {
			YYERROR;
		}
	}
	;

/*----------------------------------------------------------------------
 *	expression & part_point & point
 *----------------------------------------------------------------------*/

expression:
	expr empty_litter { $$ = $1; }

empty_litter: { empty( *litter ); }

expr:
	NUMBER	{ $$ = new Number( $1 ); }
	| ID
	{
		bool ok = false;
		if( current_wrist.len() > 0 ) {
			SymbolTable::iterator pos = 
				symbol_table().find( String($1) );
			if( pos != symbol_table().end() ) {
				$$ = new Variable( pos );
				gc_push( $$ );
				ok = true;
			}
		}
		if( !ok ) {
			char buf[200];
			sprintf( buf, "%s doesn't exist.", $1 );
			yyerror( buf );
			YYERROR;
		}		
	}
	| '('expr')' { $$ = $2;	}
	| expr '+' expr { $$ = new Addition( $1, $3 ); gc_push($$,$1,$3); }
	| expr '-' expr { $$ = new Subtraction( $1, $3 ); gc_push($$,$1,$3); }
	| expr '/' expr { $$ = new Division( $1, $3 ); gc_push($$,$1,$3); }
	| expr '*' expr	{ $$ = new Multiplication( $1, $3 ); 
			  gc_push($$,$1,$3); }
	| '+' expr %prec UNARYPLUS { $$ = $2; gc_push($$,$2); }
	| '-' expr %prec UNARYMINUS { $$ = new UnaryMinus( $2 ); 
				      gc_push($$,$2); }
	| expr '^' expr	{ $$ = new Power( $1, $3 ); gc_push($$,$1,$3); }
	| SQRT '(' expr ')' { $$ = new Sqrt( $3 ); gc_push($$,$3); }
	| SIN '(' expr ')' { $$ = new Sinus( $3 ); gc_push($$,$3); }
	| COS '(' expr ')' { $$ = new Cosinus( $3 ); gc_push($$,$3); }
	| TAN '(' expr ')' { $$ = new Tangent( $3 ); gc_push($$,$3); }
	| EXP '(' expr ')' { $$ = new Exp( $3 ); gc_push($$,$3); }
	;

part_point:
	ID '.' START
	{
		strcpy( $$.id, $1 );
		$$.endpoint = BodyBuilder::start;
	}
	| ID '.' END
	{
		strcpy( $$.id, $1 );
		$$.endpoint = BodyBuilder::end;
	}
	;

point:
	'[' expression ',' expression ',' expression ']' 
	{
		bool ok = make_point($2,$4,$6,$$);
		delete $2;
		delete $4;
		delete $6;
		if( !ok ) YYERROR;
	}
	;

%%

// ----------------------------------------------------------------------
//	static void gc_push( Expression *result, 
//			    Expression *p1, Expression *p2 );
// ----------------------------------------------------------------------

static void gc_push( Expression *result, Expression *p1, Expression *p2 )
{
	List<Expression *>::iterator pos1, pos2;

	pos1 = find( litter->begin(), litter->end(), p1 );
	if( pos1 != litter->end() ) litter->destroy( pos1 );
	pos2 = find( litter->begin(), litter->end(), p2 );
	if( pos2 != litter->end() ) litter->destroy( pos2 );

	litter->push_back( result );
}

// ----------------------------------------------------------------------
//	static bool init_wrists();
// ----------------------------------------------------------------------

static bool init_wrists()
{
	bool ok = true;
	for( List<String>::iterator name = wrist_names->begin();
		name != wrist_names->end() && ok; name++ ) {
		double	position;

		Map<String,WristMotion*>::iterator pwm =
			wrist_motions->find( *name );
		if( pwm == wrist_motions->end() ) {
			ok = false;
			char buf[100];
			sprintf( buf, "You didn't give motion for wrist %s.",
				(const char *) *name );
			yyerror( buf );
		} else {
			(*pwm)->end();
			ok = (*pwm)->evaluate( 0, position );
			if( ok ) {
				ok = body_builder->init_wrist(*name,position);
				if( !ok ) yyerror( body_builder->error() );
			} else {
				yyerror( "I can't evaluate expression at 0." );
			}
		}
	}
	
	return ok;
}
	
// ----------------------------------------------------------------------
//	static bool check_bound( double last, double next );
// ----------------------------------------------------------------------

static bool check_bound( double last, double next )
{
	bool ok = true;
	if( last > next ) {
		yyerror( "Bad range or point." );
		ok = false;
	}
	return ok;
}

// ----------------------------------------------------------------------
//	static bool add_section( const Boundaries &b, Expression *e );
// ----------------------------------------------------------------------

static bool add_section( const Boundaries &b, Expression *e )
{
	char	*expr_error = "I can't add section program.";
	char	*error_str;

	bool ok = true;
	switch( b.size ) {
	    case 1:
		if( !e ) {
			error_str = "Fixed isn't allowed for point.";
			ok = false;
		} else {
			ok = (*wrist_motions)[current_wrist]->add_point( 
				b.bounds[0], e );
			if( !ok ) error_str = expr_error;
		}
		break;
	    case 2:
		ok = (*wrist_motions)[current_wrist]->add_range( b.bounds[0],
							      b.bounds[1], e );
		if( !ok ) error_str = expr_error;
		break;
	    default:
		ERROR( "bad boundary size" );
	}
	if( !ok ) yyerror( error_str );

	return ok;
}

// ----------------------------------------------------------------------
//	static bool make_point( Expression *p1, Expression *p2,
//				Expression *p3, _Point &p )
// ----------------------------------------------------------------------

static bool make_point( Expression *p1, Expression *p2, Expression *p3, 
		       _Point &p )
{
	bool ok;

	ok = evaluate( p1, p.x );
	ok = ok && evaluate( p2, p.y );
	ok = ok && evaluate( p3, p.z );
	return ok;
}

// ----------------------------------------------------------------------
//	static bool evaluate( Expression *e, double &result )
// ----------------------------------------------------------------------

static bool evaluate( Expression *e, double &result )
{
	TEST_EXPR( e != NULL );

	bool ok = e->evaluate( result );
	if( !ok ) {
		yyerror( "I can't evaluate the expression." );
	}
	return ok;
}

// ----------------------------------------------------------------------
//	static inline SymbolTable &symbol_table()
// ----------------------------------------------------------------------

static inline SymbolTable &symbol_table()
{
	TEST_EXPR( (*wrist_motions).find(String(current_wrist)) !=
		(*wrist_motions).end() );
	return (*wrist_motions)[current_wrist]->symbol_table();
}

// ----------------------------------------------------------------------
//	void yyerror( const char *s )
// ----------------------------------------------------------------------

void yyerror( const char *s )
{
	cerr << "line:" << yyline_no << ':' << s << endl;
}

extern bool parse_input( Body &body, Map<String,WristMotion *> &wrist_motions,
			Point &s0, Viewer &viewer, FILE *input )
{
	extern int	yyparse(void);
	extern FILE	*yyin;

	TEST_EXPR( input != NULL );

	::body_builder = new BodyBuilder( viewer );
	::wrist_motions = new Map<String, WristMotion *>;
	litter = new List< Expression * >;
	wrist_names = new List< String >;

	yyin = input;
	yyline_no = 1;
	bool ok = yyparse() == 0;
	if( ok ) {
		TEST_EXPR( ::body != NULL && ::wrist_motions != NULL );
		body = *::body;
		wrist_motions = *::wrist_motions;
		s0 = speed;
	} else {
		for( Map<String, WristMotion *>::iterator w = 
		    ::wrist_motions->begin(); 
		    w != ::wrist_motions->end(); w++ ) {
			delete *w;
		}
		while( !litter->is_empty() ) {
			delete *litter->begin();
			litter->destroy( litter->begin() );
		}
	}

	delete wrist_names;
	delete litter;
	delete ::body;
	delete ::wrist_motions;

	return ok;
}

// ----------------------------------------------------------------------
/*
#include	<unistd.h>

int main()
{
	Viewer	viewer;
	Body 	body( viewer );
	Map<String, WristMotion *> wrist_motions;

	bool ok = parse_input( body, wrist_motions, viewer );

	if( ok ) {
		God	god( body, Land(viewer) );
		sleep( 5 );
		for( double t = 0; t <= 10; t += 0.01 ) {
			God::Metamorphoses motions( wrist_motions.size() );
			bool ok = true;
			int i = 0;
			for( Map<String, WristMotion *>::iterator w = 
			    wrist_motions.begin(); 
			    w != wrist_motions.end() && ok;
			    w++ ) {
				double value;
				ok = ok && (*w)->evaluate( t, value );
				motions[i++] = God::Motion( 
					wrist_motions.key(w), value );
			}
			viewer.begin();
			god.land().draw();
			god.body().draw();
			viewer.end();
			god.time_elapsed( 0.01, motions );
			usleep( 10 );
		}
		sleep( 100 );
	}

	return 0;
}
*/
