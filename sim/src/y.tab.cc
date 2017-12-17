#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define yyclearin (yychar=(-1))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING (yyerrflag!=0)
#define YYPREFIX "yy"
#line 3 "y.tab.y"

#include	"bodyspec.hh"
#include	<string.h>

extern int yylex(void);

int	yyline_no;	/* to count input lines*/

/* private variables*/

static Map<String,WristMotion*>	*wrist_motions = 0;
static Body			*body = 0;

static BodyBuilder	*body_builder = 0;
static String		current_wrist;
static List<String>	*wrist_names = 0;
static bool		first_boundary;
static double		last_point;
static Point		speed; /* start speed*/

static List<Expression *>	*litter = 0;

/* private functions*/

static void gc_push( Expression *result, Expression *p1=0, Expression *p2=0 );
static bool init_wrists();
static bool check_bound( double last, double next );
static bool add_section( const Boundaries &b, Expression *e );
static bool make_point( Expression *p1, Expression *p2, Expression *p3, 
		       _Point &p );
static bool evaluate( Expression *e, double &result );
static inline SymbolTable &symbol_table();
static void yyerror( const char *s );

#line 39 "y.tab.y"
typedef union {
	Expression	*expr;
	Boundaries	boundaries;
	_Point		point;
	PartPoint	part_point;
	double		number;
	bool		route;
	char		id[MAX_ID_LEN+1];
} YYSTYPE;
#line 57 "y.tab.c"
#define BODY 257
#define PARTS 258
#define WRISTS 259
#define INIT 260
#define PROGRAM 261
#define FIXED 262
#define START 263
#define END 264
#define SQRT 265
#define SIN 266
#define COS 267
#define TAN 268
#define EXP 269
#define NUMBER 270
#define ID 271
#define ROUTE 272
#define UNARYPLUS 273
#define UNARYMINUS 274
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    0,   11,   10,   10,   12,    5,   13,   14,   14,   15,
    4,    4,    4,    3,    3,    9,    9,   16,    8,    8,
   17,    1,   18,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    7,    7,
    6,
};
short yylen[] = {                                         2,
   10,    6,    1,    2,    4,    1,    0,    1,    2,    4,
    1,    2,    3,    1,    1,    1,    2,    7,    1,    2,
    9,    2,    0,    1,    1,    3,    3,    3,    3,    3,
    2,    2,    3,    4,    4,    4,    4,    4,    3,    3,
    7,
};
short yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   20,
    0,    0,    0,    0,    0,   24,   25,    0,    0,    0,
    0,    0,    0,    0,   16,    0,    0,    0,    0,    0,
   31,   32,    0,    0,    0,    0,    0,    0,    0,   22,
    0,    0,   17,    0,    0,    0,    0,    0,   26,    0,
    0,    0,    0,    0,    0,    0,    0,    6,    0,    0,
    3,   34,   35,   36,   37,   38,    0,    0,    0,    7,
    0,    4,    0,   39,   40,    0,    0,    0,    0,    1,
    0,    0,    0,    0,    0,    5,    0,    0,    0,   21,
   18,    0,   12,    0,    9,    0,    0,   13,   14,   15,
    0,    0,    0,   10,    0,    0,    0,    2,   41,
};
short yydgoto[] = {                                       2,
   21,   22,  101,   85,   59,   79,   57,    6,   24,   60,
   80,   61,   77,   86,   87,   25,    7,   40,
};
short yysindex[] = {                                   -239,
 -256,    0, -245, -243,  -29, -225, -243,  -33, -235,    0,
   -2,   -1,    7,   29,   45,    0,    0,  -33,  -33,  -33,
   -4,   48,  -17, -255,    0,  -33,  -33,  -33,  -33,  -33,
    0,    0,  -10,  -33,  -33,  -33,  -33,  -33,  -33,    0,
 -179, -175,    0,   36,   62,   70,   77,   84,    0,   55,
   40,   40,    6,    6,    6,   56,   64,    0,   58, -251,
    0,    0,    0,    0,    0,    0,  -33, -240, -179,    0,
   37,    0,   88,    0,    0,   89,  -31,  -33, -179,    0,
 -151,   37,   90, -134,   79,    0,  -31,   94,   81,    0,
    0, -130,    0,  -40,    0,  -33,   37,    0,    0,    0,
   82,   99,  100,    0,  -33,  -33,   52,    0,    0,
};
short yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,    0, -113,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   17,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   13,   30,    1,    8,   21,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   95,    0,    0,    0, -249,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,
};
short yygindex[] = {                                      0,
   -8,  131,    0,    0,    0,  -78,  -54,  140,    0,    0,
    0,   92,    0,   61,    0,  130,    0,    0,
};
#define YYTABLESIZE 239
short yytable[] = {                                      20,
   30,    3,   18,   91,   19,   42,   20,   29,   71,   18,
    8,   19,   27,   84,   76,   23,   23,    1,  103,   58,
   33,    8,   74,   75,   89,   50,    4,    5,    8,   28,
   49,   37,   35,    9,   36,   23,   38,   26,   27,   34,
   41,   30,   30,   30,   30,   30,   28,   30,   29,   29,
   29,   29,   29,   27,   29,   27,   27,   27,   73,   30,
   23,   33,   33,   33,   33,   33,   29,   33,   29,   88,
   28,   27,   28,   28,   28,   23,   62,   37,   35,   33,
   36,   37,   38,   39,   30,  100,   38,  102,   28,   37,
   35,   56,   36,   30,   38,   58,  107,  108,   67,   39,
   29,   68,   63,   37,   35,   27,   36,   69,   38,   23,
   64,   37,   35,   33,   36,   70,   38,   65,   37,   35,
   90,   36,   28,   38,   66,   37,   35,   78,   36,   39,
   38,   81,   82,   39,   92,   93,   94,   96,   97,   98,
  104,   39,  105,  106,  109,   19,   10,   95,   31,   32,
   33,   72,   11,   43,    0,   39,   44,   45,   46,   47,
   48,    0,    0,   39,    0,   51,   52,   53,   54,   55,
   39,    0,    0,    0,    0,    0,    0,   39,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   99,    0,    0,   11,   12,   13,   14,   15,   16,
   17,   11,   12,   13,   14,   15,   16,   17,   83,
};
short yycheck[] = {                                      40,
    0,  258,   43,   82,   45,  261,   40,    0,  260,   43,
  260,   45,    0,   45,   69,  271,    0,  257,   97,  271,
    0,  271,  263,  264,   79,   34,  272,  271,   58,    0,
   41,   42,   43,  259,   45,  271,   47,   40,   40,   44,
   58,   41,   42,   43,   44,   45,   40,   47,   41,   42,
   43,   44,   45,   41,   47,   43,   44,   45,   67,   59,
   44,   41,   42,   43,   44,   45,   59,   47,   40,   78,
   41,   59,   43,   44,   45,   59,   41,   42,   43,   59,
   45,   42,   47,   94,   40,   94,   47,   96,   59,   42,
   43,  271,   45,   93,   47,  271,  105,  106,   44,   94,
   93,   46,   41,   42,   43,   93,   45,   44,   47,   93,
   41,   42,   43,   93,   45,   58,   47,   41,   42,   43,
  272,   45,   93,   47,   41,   42,   43,   91,   45,   94,
   47,   44,   44,   94,   45,  270,   58,   44,   58,  270,
   59,   94,   44,   44,   93,  259,    7,   87,   18,   19,
   20,   60,   58,   24,   -1,   94,   26,   27,   28,   29,
   30,   -1,   -1,   94,   -1,   35,   36,   37,   38,   39,
   94,   -1,   -1,   -1,   -1,   -1,   -1,   94,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  262,   -1,   -1,  265,  266,  267,  268,  269,  270,
  271,  265,  266,  267,  268,  269,  270,  271,  270,
};
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 274
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'('","')'","'*'","'+'","','","'-'","'.'","'/'",0,0,0,0,0,0,0,0,0,0,
"':'","';'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'['",
0,"']'","'^'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,"BODY","PARTS","WRISTS","INIT","PROGRAM","FIXED","START",
"END","SQRT","SIN","COS","TAN","EXP","NUMBER","ID","ROUTE","UNARYPLUS",
"UNARYMINUS",
};
char *yyrule[] = {
"$accept : s",
"s : BODY PARTS ROUTE body_parts WRISTS wrist_parts PROGRAM program_parts INIT init_part",
"init_part : point part_point ':' point ',' expression",
"program_parts : program",
"program_parts : program_parts program",
"program : wrist_id ':' begin_sections sections",
"wrist_id : ID",
"begin_sections :",
"sections : section",
"sections : section sections",
"section : boundaries ':' section_program ';'",
"boundaries : NUMBER",
"boundaries : '-' NUMBER",
"boundaries : NUMBER '-' NUMBER",
"section_program : FIXED",
"section_program : expression",
"wrist_parts : wrist",
"wrist_parts : wrist_parts wrist",
"wrist : ID ':' part_point ',' part_point ',' point",
"body_parts : part",
"body_parts : part body_parts",
"part : ID ':' expression ',' expression ',' expression ',' ROUTE",
"expression : expr empty_litter",
"empty_litter :",
"expr : NUMBER",
"expr : ID",
"expr : '(' expr ')'",
"expr : expr '+' expr",
"expr : expr '-' expr",
"expr : expr '/' expr",
"expr : expr '*' expr",
"expr : '+' expr",
"expr : '-' expr",
"expr : expr '^' expr",
"expr : SQRT '(' expr ')'",
"expr : SIN '(' expr ')'",
"expr : COS '(' expr ')'",
"expr : TAN '(' expr ')'",
"expr : EXP '(' expr ')'",
"part_point : ID '.' START",
"part_point : ID '.' END",
"point : '[' expression ',' expression ',' expression ']'",
};
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH 500
#endif
#endif
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short yyss[YYSTACKSIZE];
YYSTYPE yyvs[YYSTACKSIZE];
#define yystacksize YYSTACKSIZE
#line 368 "y.tab.y"

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
#line 510 "y.tab.c"
#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse()
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register char *yys;
    extern char *getenv();

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if (yyn = yydefred[yystate]) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yyss + yystacksize - 1)
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#ifdef lint
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yyss + yystacksize - 1)
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 1:
#line 91 "y.tab.y"
{
		TEST_EXPR( body_builder != NULL & wrist_motions != NULL );

		body = (*body_builder)( yyvsp[-7].route );
		delete body_builder;
		body_builder = NULL;
		if( !body ) {
			yyerror( "I can't create body." );
			YYERROR;
		}
	}
break;
case 2:
#line 110 "y.tab.y"
{
		speed = Point(yyvsp[-5].point.x, yyvsp[-5].point.y, yyvsp[-5].point.z);
		bool ok = init_wrists();
		if( ok ) {
			double angle;
			ok = evaluate( yyvsp[0].expr, angle );
			if( ok ) {
				ok = body_builder->init( yyvsp[-4].part_point.id, yyvsp[-4].part_point.endpoint,
					Point(yyvsp[-2].point.x, yyvsp[-2].point.y, yyvsp[-2].point.z), angle );
			}
			if( !ok ) yyerror( body_builder->error() );
		}
		delete yyvsp[0].expr;
		if( !ok ) YYERROR;
	}
break;
case 6:
#line 141 "y.tab.y"
{
		List<String>::iterator pos = 
			find( wrist_names->begin(), wrist_names->end(),
				String(yyvsp[0].id) );
		if( pos == wrist_names->end() ) {
			char buf[200];
			sprintf( buf, "Wrist %s doesn't exist.", yyvsp[0].id );
			yyerror( buf );
			YYERROR;
		}
		Map<String,WristMotion*>::iterator pwm = 
			wrist_motions->find( yyvsp[0].id );
		if( pwm != wrist_motions->end() ) {
			char buf[200];
			sprintf(buf,"Motion of wrist %s is already defined.",
				yyvsp[0].id );
			yyerror( buf );
			YYERROR;
		}
		current_wrist = yyvsp[0].id;
		WristMotion *wm = new WristMotion;
		wm->start( 0 );
		wm->symbol_table()[ "t" ] = 0;
		(*wrist_motions)[ current_wrist ] = wm;

		strcpy( yyval.id, yyvsp[0].id );
	}
break;
case 7:
#line 170 "y.tab.y"
{ first_boundary = true; last_point = 0; }
break;
case 10:
#line 179 "y.tab.y"
{
		bool ok = add_section( yyvsp[-3].boundaries, yyvsp[-1].expr );
		if( !ok ) YYERROR;
	}
break;
case 11:
#line 187 "y.tab.y"
{
		bool ok;
		if( first_boundary && yyvsp[0].number != 0 ) {
			yyerror("Section must start with 0 parameter value.");
			ok = false;
		} else {
			ok = check_bound( last_point, yyvsp[0].number );
		}
		if( !ok ) YYERROR;

		yyval.boundaries.size = 1;
		yyval.boundaries.bounds[0] = yyvsp[0].number;
		first_boundary = false;
		last_point = yyvsp[0].number;
	}
break;
case 12:
#line 203 "y.tab.y"
{
		if( check_bound( last_point, yyvsp[0].number ) ) {
			yyval.boundaries.size = 2;
			yyval.boundaries.bounds[0] = last_point;
			yyval.boundaries.bounds[1] = yyvsp[0].number;
		} else {
			YYERROR;
		}
		first_boundary = false;
		last_point = yyvsp[0].number;
	}
break;
case 13:
#line 215 "y.tab.y"
{
		bool ok = true;
		if( first_boundary && yyvsp[-2].number != 0 ) {
			yyerror("Section must start with 0 parameter value.");
		}
		if( check_bound(last_point, yyvsp[-2].number) && check_bound(yyvsp[-2].number,yyvsp[0].number) ) {
			yyval.boundaries.size = 2;
			yyval.boundaries.bounds[0] = yyvsp[-2].number;
			yyval.boundaries.bounds[1] = yyvsp[0].number;
		} else {
			ok = false;
		}
		if( !ok ) YYERROR;

		first_boundary = false;
		last_point = yyvsp[0].number;
	}
break;
case 14:
#line 235 "y.tab.y"
{ yyval.expr = 0; }
break;
case 15:
#line 236 "y.tab.y"
{ TEST_EXPR( yyvsp[0].expr != NULL ); yyval.expr = yyvsp[0].expr; }
break;
case 18:
#line 250 "y.tab.y"
{
		bool ok = body_builder->add_wrist( yyvsp[-6].id,
				yyvsp[-4].part_point.id, yyvsp[-4].part_point.endpoint, yyvsp[-2].part_point.id, yyvsp[-2].part_point.endpoint,
				Point( yyvsp[0].point.x, yyvsp[0].point.y, yyvsp[0].point.z ) );
		if( !ok ) {
			yyerror( body_builder->error() );
			YYERROR;
		}
		wrist_names->push_back( yyvsp[-6].id );
	}
break;
case 21:
#line 277 "y.tab.y"
{
		double	l, w, c;
		bool ok = evaluate( yyvsp[-6].expr, l );
		delete yyvsp[-6].expr;
		ok = ok && evaluate( yyvsp[-4].expr, w );
		delete yyvsp[-4].expr;
		ok = ok && evaluate( yyvsp[-2].expr, c );
		delete yyvsp[-2].expr;

		if( ok ) {
			ok = body_builder->add_part( yyvsp[-8].id, l, w, c, yyvsp[0].route );
			if( !ok ) yyerror( body_builder->error() );
		}
		if( !ok ) {
			YYERROR;
		}
	}
break;
case 22:
#line 301 "y.tab.y"
{ yyval.expr = yyvsp[-1].expr; }
break;
case 23:
#line 303 "y.tab.y"
{ empty( *litter ); }
break;
case 24:
#line 306 "y.tab.y"
{ yyval.expr = new Number( yyvsp[0].number ); }
break;
case 25:
#line 308 "y.tab.y"
{
		bool ok = false;
		if( current_wrist.len() > 0 ) {
			SymbolTable::iterator pos = 
				symbol_table().find( String(yyvsp[0].id) );
			if( pos != symbol_table().end() ) {
				yyval.expr = new Variable( pos );
				gc_push( yyval.expr );
				ok = true;
			}
		}
		if( !ok ) {
			char buf[200];
			sprintf( buf, "%s doesn't exist.", yyvsp[0].id );
			yyerror( buf );
			YYERROR;
		}		
	}
break;
case 26:
#line 326 "y.tab.y"
{ yyval.expr = yyvsp[-1].expr;	}
break;
case 27:
#line 327 "y.tab.y"
{ yyval.expr = new Addition( yyvsp[-2].expr, yyvsp[0].expr ); gc_push(yyval.expr,yyvsp[-2].expr,yyvsp[0].expr); }
break;
case 28:
#line 328 "y.tab.y"
{ yyval.expr = new Subtraction( yyvsp[-2].expr, yyvsp[0].expr ); gc_push(yyval.expr,yyvsp[-2].expr,yyvsp[0].expr); }
break;
case 29:
#line 329 "y.tab.y"
{ yyval.expr = new Division( yyvsp[-2].expr, yyvsp[0].expr ); gc_push(yyval.expr,yyvsp[-2].expr,yyvsp[0].expr); }
break;
case 30:
#line 330 "y.tab.y"
{ yyval.expr = new Multiplication( yyvsp[-2].expr, yyvsp[0].expr ); 
			  gc_push(yyval.expr,yyvsp[-2].expr,yyvsp[0].expr); }
break;
case 31:
#line 332 "y.tab.y"
{ yyval.expr = yyvsp[0].expr; gc_push(yyval.expr,yyvsp[0].expr); }
break;
case 32:
#line 333 "y.tab.y"
{ yyval.expr = new UnaryMinus( yyvsp[0].expr ); 
				      gc_push(yyval.expr,yyvsp[0].expr); }
break;
case 33:
#line 335 "y.tab.y"
{ yyval.expr = new Power( yyvsp[-2].expr, yyvsp[0].expr ); gc_push(yyval.expr,yyvsp[-2].expr,yyvsp[0].expr); }
break;
case 34:
#line 336 "y.tab.y"
{ yyval.expr = new Sqrt( yyvsp[-1].expr ); gc_push(yyval.expr,yyvsp[-1].expr); }
break;
case 35:
#line 337 "y.tab.y"
{ yyval.expr = new Sinus( yyvsp[-1].expr ); gc_push(yyval.expr,yyvsp[-1].expr); }
break;
case 36:
#line 338 "y.tab.y"
{ yyval.expr = new Cosinus( yyvsp[-1].expr ); gc_push(yyval.expr,yyvsp[-1].expr); }
break;
case 37:
#line 339 "y.tab.y"
{ yyval.expr = new Tangent( yyvsp[-1].expr ); gc_push(yyval.expr,yyvsp[-1].expr); }
break;
case 38:
#line 340 "y.tab.y"
{ yyval.expr = new Exp( yyvsp[-1].expr ); gc_push(yyval.expr,yyvsp[-1].expr); }
break;
case 39:
#line 345 "y.tab.y"
{
		strcpy( yyval.part_point.id, yyvsp[-2].id );
		yyval.part_point.endpoint = BodyBuilder::start;
	}
break;
case 40:
#line 350 "y.tab.y"
{
		strcpy( yyval.part_point.id, yyvsp[-2].id );
		yyval.part_point.endpoint = BodyBuilder::end;
	}
break;
case 41:
#line 358 "y.tab.y"
{
		bool ok = make_point(yyvsp[-5].expr,yyvsp[-3].expr,yyvsp[-1].expr,yyval.point);
		delete yyvsp[-5].expr;
		delete yyvsp[-3].expr;
		delete yyvsp[-1].expr;
		if( !ok ) YYERROR;
	}
break;
#line 928 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yyss + yystacksize - 1)
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
