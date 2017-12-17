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
#line 3 "logo.y"

#include	<stdio.h>
#include	"logo.h"

extern int	yylex();
extern int	line_no;

#define	MAX_ID_LEN	30

#line 14 "logo.y"
typedef union{
	char		id[MAX_ID_LEN+1];
	double		number;
	Array<String>	*par_names;
	int		par_num;
} YYSTYPE;
#line 29 "y.tab.c"
#define NUMBER 257
#define WHILE 258
#define IF 259
#define THEN 260
#define ELSE 261
#define FUNCTION 262
#define END 263
#define ID 264
#define SIN 265
#define COS 266
#define TAN 267
#define ACOS 268
#define ATAN 269
#define FORWARD 270
#define BACKWARD 271
#define LEFT 272
#define RIGHT 273
#define PEN 274
#define UP 275
#define DOWN 276
#define PRINT 277
#define DIRECTION 278
#define POSITION 279
#define LE 280
#define GE 281
#define UNARYMINUS 282
#define UNARYPLUS 283
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    0,    6,    7,    7,    7,    7,    8,    8,    8,    9,
   10,   10,   10,   11,   12,   13,   14,   14,   20,    5,
    1,    1,    2,    2,   15,   17,   21,   19,   19,   19,
   19,   19,   19,   19,   19,   19,   19,   19,   19,   19,
   19,   19,   19,   19,   19,   16,   16,   16,   16,   16,
   16,    3,    3,    4,    4,   18,   18,   18,   18,   18,
   18,   18,   18,   18,
};
short yylen[] = {                                         2,
    2,    0,    0,    2,    3,    3,    1,    5,    5,    6,
    1,    1,    1,    1,    1,    4,    0,    4,    0,    4,
    0,    1,    1,    3,    0,    4,    1,    1,    1,    3,
    3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    2,    2,    1,    4,    4,    4,    4,    4,
    4,    0,    1,    1,    3,    2,    2,    2,    2,    2,
    2,    2,    2,    4,
};
short yydefred[] = {                                      2,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    1,    0,    0,    7,   11,   12,   13,    0,   28,    0,
    0,    0,    0,   45,    0,   29,   19,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   61,   62,    0,    0,    0,    4,    0,    0,    0,   44,
   43,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   25,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    5,    6,
    0,   42,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   32,   33,   34,    0,    0,    0,    0,    0,
   22,    0,   46,    0,   47,   48,   49,   50,   51,    0,
    0,    8,    9,    0,    0,    0,   20,    0,    0,    0,
   16,   24,   10,   18,
};
short yydgoto[] = {                                       1,
  110,  111,   80,   81,   40,    2,   21,   22,   23,   24,
   33,   37,   76,  106,  112,   34,   26,   27,   35,   77,
   36,
};
short yysindex[] = {                                      0,
    0,   -5,  138,  138, -261,  -17,    5,    9,   15,   16,
   21,  138,  138,  138,  138, -246,  138,  138,  138,   -5,
    0,   30,   52,    0,    0,    0,    0,  -45,    0,  138,
  138,  138,   53,    0,  351,    0,    0,  351,   24,   55,
  138,  138,  138,  138,  138,  138,  351,  351,  351,  351,
    0,    0,  351,  351,  111,    0,   -5,   -5,    6,    0,
    0,  152,   -5,  138,  138,  138,  138,  138,  138,  138,
  138,  138,  138,  138,  138,    0, -192, -194,    0,   34,
   32,  351,  178,  244,  284,  313,  339,  138,    0,    0,
  138,    0, -184,  187,  -36,  -27,  -27,  -27,  -27,  -27,
  -25,  -25,    0,    0,    0, -180, -176,   76,   47,   54,
    0,   -5,    0,  138,    0,    0,    0,    0,    0,  351,
  351,    0,    0,   84,   -5, -194,    0, -164,  351,   -5,
    0,    0,    0,    0,
};
short yyrindex[] = {                                      0,
    0,  100,    0,    0,    0,  -10,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   14,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   91,    0,    0, -158,    0,    0,
   67,    0,    0,    0,    0,    0,   30,   52,  101,  105,
    0,    0,  107,   53,    0,    0,   14,   14,    0,    0,
    0,    0, -145,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -259,    0,   78,    0,    0,
   90,    3,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   97,   83,   36,   43,   68,   72,   82,
   -2,   28,    0,    0,    0,    0,    0,    0,   95,    0,
    0, -145,    0,    0,    0,    0,    0,    0,    0,  115,
  127,    0,    0,    0, -242,    0,    0,    0,   13, -145,
    0,    0,    0,    0,
};
short yygindex[] = {                                      0,
    0,   19,    0,    0,    0,    0,  390,    0,    0,    0,
    0,    0,    0,    0,    0,  359,    0,    0,  468,   63,
  367,
};
#define YYTABLESIZE 632
short yytable[] = {                                      27,
   75,   19,   39,   17,   20,   73,   71,   30,   72,   75,
   74,   75,   59,    3,   73,   71,   73,   72,    3,   74,
    3,   74,   41,   66,   68,   67,   27,   27,   51,   52,
   27,   27,   27,   27,   27,   30,   27,   31,   30,   57,
   30,   30,   30,   54,   42,   36,   54,   27,   43,   27,
   27,   27,   38,   55,   44,   45,   55,   30,   30,   30,
   46,   58,   63,   78,   79,   31,   91,  108,   31,  109,
   31,   31,   31,   36,  113,  114,   36,   35,  122,   36,
   38,   37,  123,   38,  124,  125,   38,   31,   31,   31,
  126,   39,   41,  130,  127,   36,   36,   36,  133,    3,
   14,   15,   38,   38,   38,   35,   40,   52,   35,   37,
   59,   35,   37,   27,   60,   37,   56,    3,   21,   39,
   41,   30,   39,   41,   64,   39,   41,   35,   35,   35,
   53,   37,   37,   37,   40,   23,   26,   40,  107,    0,
   40,   39,   39,   39,  132,    0,    0,   75,   64,    0,
    0,   31,   73,   71,   88,   72,    0,   74,    0,   36,
    0,    0,    0,    0,    0,    0,   38,    0,    0,    0,
   66,   68,   67,    0,    0,    0,    0,   32,    0,    0,
   30,    0,   31,    0,    0,    0,    0,    0,   75,   64,
    0,   35,   92,   73,   71,   37,   72,    0,   74,    0,
    0,    0,    0,    0,    0,   39,   41,    0,    0,    0,
    0,   66,   68,   67,   75,   64,    0,    0,  115,   73,
   71,    0,   72,   75,   74,    0,    0,    0,   73,   71,
    0,   72,    0,   74,   65,    0,    0,   66,   68,   67,
    0,    0,    0,   69,   70,    0,   66,   68,   67,   27,
    0,    0,    3,    4,    0,    0,    5,   30,    6,    7,
    8,    9,   10,   11,   12,   13,   14,   15,   16,   27,
   27,   17,   18,   19,    3,   65,    3,   30,   30,    0,
   75,   64,    0,    0,  116,   73,   71,   31,   72,    0,
   74,    0,    0,    0,    0,   36,    0,    0,    0,    0,
    0,   65,   38,   66,   68,   67,    0,   31,   31,    0,
   65,    0,    0,    0,    0,   36,   36,    0,    0,    0,
   75,   64,   38,   38,  117,   73,   71,   35,   72,    0,
   74,   37,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   39,   41,   66,   68,   67,    0,   35,   35,   75,
   64,   37,   37,  118,   73,   71,   40,   72,    0,   74,
   25,   39,   39,    0,    0,    0,    0,   65,   28,    0,
    0,    0,   66,   68,   67,   75,   64,    0,   25,  119,
   73,   71,    0,   72,    0,   74,   28,   75,   64,    0,
   69,   70,   73,   71,   29,   72,    0,   74,   66,   68,
   67,    6,    7,    8,    9,   10,   11,   65,    0,   56,
   66,   68,   67,    0,    0,   25,   25,    0,    0,    0,
    0,   25,    0,   28,   28,    0,    0,    0,    0,   28,
    0,   69,   70,    0,    0,    0,   65,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   89,   90,    0,    0,
    0,    0,   93,    0,    0,    0,    0,   69,   70,    0,
    0,    0,   65,    0,    0,    0,   69,   70,    0,    0,
   25,   38,    0,    0,   65,    0,    0,    0,   28,   47,
   48,   49,   50,   25,   53,   54,   55,    0,   25,    0,
    0,   28,    0,    0,    0,    0,   28,   60,   61,   62,
    0,  128,    0,    0,    0,    0,    0,    0,   82,   83,
   84,   85,   86,   87,  131,    0,    0,    0,    0,  134,
    0,    0,    0,   69,   70,    0,    0,    0,    0,    0,
    0,   94,   95,   96,   97,   98,   99,  100,  101,  102,
  103,  104,  105,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  120,    0,    0,  121,    0,
    0,    0,    0,   69,   70,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  129,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   69,   70,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   69,   70,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   69,   70,
};
short yycheck[] = {                                      10,
   37,  261,  264,  263,   10,   42,   43,   10,   45,   37,
   47,   37,   58,    0,   42,   43,   42,   45,  261,   47,
  263,   47,   40,   60,   61,   62,   37,   38,  275,  276,
   41,   42,   43,   44,   45,   38,   47,   10,   41,   10,
   43,   44,   45,   41,   40,   10,   44,   58,   40,   60,
   61,   62,   10,   41,   40,   40,   44,   60,   61,   62,
   40,   10,   10,   40,   10,   38,   61,  260,   41,  264,
   43,   44,   45,   38,   41,   44,   41,   10,  263,   44,
   38,   10,  263,   41,  261,   10,   44,   60,   61,   62,
   44,   10,   10,   10,   41,   60,   61,   62,  263,    0,
   10,  260,   60,   61,   62,   38,   10,   41,   41,   38,
   10,   44,   41,  124,   10,   44,   10,  263,   41,   38,
   38,  124,   41,   41,   10,   44,   44,   60,   61,   62,
   41,   60,   61,   62,   38,   41,   10,   41,   76,   -1,
   44,   60,   61,   62,  126,   -1,   -1,   37,   38,   -1,
   -1,  124,   42,   43,   44,   45,   -1,   47,   -1,  124,
   -1,   -1,   -1,   -1,   -1,   -1,  124,   -1,   -1,   -1,
   60,   61,   62,   -1,   -1,   -1,   -1,   40,   -1,   -1,
   43,   -1,   45,   -1,   -1,   -1,   -1,   -1,   37,   38,
   -1,  124,   41,   42,   43,  124,   45,   -1,   47,   -1,
   -1,   -1,   -1,   -1,   -1,  124,  124,   -1,   -1,   -1,
   -1,   60,   61,   62,   37,   38,   -1,   -1,   41,   42,
   43,   -1,   45,   37,   47,   -1,   -1,   -1,   42,   43,
   -1,   45,   -1,   47,  124,   -1,   -1,   60,   61,   62,
   -1,   -1,   -1,  280,  281,   -1,   60,   61,   62,  260,
   -1,   -1,  258,  259,   -1,   -1,  262,  260,  264,  265,
  266,  267,  268,  269,  270,  271,  272,  273,  274,  280,
  281,  277,  278,  279,  261,  124,  263,  280,  281,   -1,
   37,   38,   -1,   -1,   41,   42,   43,  260,   45,   -1,
   47,   -1,   -1,   -1,   -1,  260,   -1,   -1,   -1,   -1,
   -1,  124,  260,   60,   61,   62,   -1,  280,  281,   -1,
  124,   -1,   -1,   -1,   -1,  280,  281,   -1,   -1,   -1,
   37,   38,  280,  281,   41,   42,   43,  260,   45,   -1,
   47,  260,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  260,  260,   60,   61,   62,   -1,  280,  281,   37,
   38,  280,  281,   41,   42,   43,  260,   45,   -1,   47,
    2,  280,  281,   -1,   -1,   -1,   -1,  124,    2,   -1,
   -1,   -1,   60,   61,   62,   37,   38,   -1,   20,   41,
   42,   43,   -1,   45,   -1,   47,   20,   37,   38,   -1,
  280,  281,   42,   43,  257,   45,   -1,   47,   60,   61,
   62,  264,  265,  266,  267,  268,  269,  124,   -1,   20,
   60,   61,   62,   -1,   -1,   57,   58,   -1,   -1,   -1,
   -1,   63,   -1,   57,   58,   -1,   -1,   -1,   -1,   63,
   -1,  280,  281,   -1,   -1,   -1,  124,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   57,   58,   -1,   -1,
   -1,   -1,   63,   -1,   -1,   -1,   -1,  280,  281,   -1,
   -1,   -1,  124,   -1,   -1,   -1,  280,  281,   -1,   -1,
  112,    4,   -1,   -1,  124,   -1,   -1,   -1,  112,   12,
   13,   14,   15,  125,   17,   18,   19,   -1,  130,   -1,
   -1,  125,   -1,   -1,   -1,   -1,  130,   30,   31,   32,
   -1,  112,   -1,   -1,   -1,   -1,   -1,   -1,   41,   42,
   43,   44,   45,   46,  125,   -1,   -1,   -1,   -1,  130,
   -1,   -1,   -1,  280,  281,   -1,   -1,   -1,   -1,   -1,
   -1,   64,   65,   66,   67,   68,   69,   70,   71,   72,
   73,   74,   75,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   88,   -1,   -1,   91,   -1,
   -1,   -1,   -1,  280,  281,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  114,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  280,  281,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  280,  281,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  280,  281,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 283
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,"'\\n'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'%'","'&'",0,"'('","')'","'*'","'+'","','","'-'",0,"'/'",0,0,0,0,0,
0,0,0,0,0,"':'",0,"'<'","'='","'>'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"'|'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"NUMBER","WHILE","IF","THEN","ELSE","FUNCTION",
"END","ID","SIN","COS","TAN","ACOS","ATAN","FORWARD","BACKWARD","LEFT","RIGHT",
"PEN","UP","DOWN","PRINT","DIRECTION","POSITION","LE","GE","UNARYMINUS",
"UNARYPLUS",
};
char *yyrule[] = {
"$accept : S",
"S : init command_list",
"init :",
"command_list :",
"command_list : '\\n' command_list",
"command_list : command '\\n' command_list",
"command_list : function_def '\\n' command_list",
"command : simple_command",
"command : WHILE while_expression '\\n' command_list END",
"command : IF if_expression then_branch else_branch END",
"function_def : FUNCTION function_head '\\n' init_function command_list END",
"simple_command : function_call",
"simple_command : assignment",
"simple_command : turtle_command",
"while_expression : expression",
"if_expression : expression",
"then_branch : init_if THEN '\\n' command_list",
"else_branch :",
"else_branch : init_if ELSE '\\n' command_list",
"init_if :",
"function_head : ID '(' fparameters ')'",
"fparameters :",
"fparameters : fparameters2",
"fparameters2 : ID",
"fparameters2 : ID ',' fparameters2",
"init_function :",
"assignment : variable ':' '=' expression",
"variable : ID",
"expression : NUMBER",
"expression : variable",
"expression : expression '+' expression",
"expression : expression '-' expression",
"expression : expression '*' expression",
"expression : expression '/' expression",
"expression : expression '%' expression",
"expression : expression '=' expression",
"expression : expression '<' expression",
"expression : expression LE expression",
"expression : expression '>' expression",
"expression : expression GE expression",
"expression : expression '&' expression",
"expression : expression '|' expression",
"expression : '(' expression ')'",
"expression : '-' expression",
"expression : '+' expression",
"expression : function_call",
"function_call : ID '(' cparameters ')'",
"function_call : SIN '(' expression ')'",
"function_call : COS '(' expression ')'",
"function_call : TAN '(' expression ')'",
"function_call : ACOS '(' expression ')'",
"function_call : ATAN '(' expression ')'",
"cparameters :",
"cparameters : cparameters2",
"cparameters2 : expression",
"cparameters2 : cparameters2 ',' expression",
"turtle_command : PRINT expression",
"turtle_command : FORWARD expression",
"turtle_command : BACKWARD expression",
"turtle_command : LEFT expression",
"turtle_command : RIGHT expression",
"turtle_command : PEN UP",
"turtle_command : PEN DOWN",
"turtle_command : DIRECTION expression",
"turtle_command : POSITION expression ',' expression",
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
#line 328 "logo.y"

void yyerror( char *s )
{
	fprintf( stderr, "line %d: %s\n", line_no, s );
}
#line 382 "y.tab.c"
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
case 2:
#line 42 "logo.y"
{
		expr = new Expression();
		block = new Stack< Ptr<Sequence> >();
		while_stack = new Stack< Ptr<While> >();
		if_stack = new Stack< Ptr<If> >();
		block->push( new Sequence() );
	}
break;
case 7:
#line 61 "logo.y"
{
			block->top()->add( new Command(expr) );
			expr = new Expression();
		}
break;
case 8:
#line 66 "logo.y"
{
			while_stack->top()->body( block->pop() );
			block->top()->add( (While *) while_stack->pop() );
		}
break;
case 9:
#line 71 "logo.y"
{
			block->top()->add( (If *) if_stack->pop() );
		}
break;
case 10:
#line 78 "logo.y"
{
			try {
				proc_symbol_table->define( yyvsp[-4].id, block->pop() );
			} 
			catch( const ProcSymbolTable::DEFINED & ) {
				char	buf[100];
				sprintf( buf, "function %s is already defined",
					yyvsp[-4].id );
				yyerror( buf );
				YYERROR;
			}
		}
break;
case 14:
#line 107 "logo.y"
{
				while_stack->push( new While() );
				block->push( new Sequence() );

				while_stack->top()->condition( expr );
				expr = new Expression();
			}
break;
case 15:
#line 121 "logo.y"
{
			if_stack->push( new If() );
			if_stack->top()->condition( expr );
			expr = new Expression();
		}
break;
case 16:
#line 129 "logo.y"
{
			if_stack->top()->then_branch( block->pop() );
		}
break;
case 18:
#line 136 "logo.y"
{
			if_stack->top()->else_branch( block->pop() );
		}
break;
case 19:
#line 141 "logo.y"
{ block->push( new Sequence() ); }
break;
case 20:
#line 149 "logo.y"
{
			proc_symbol_table->declare( yyvsp[-3].id, *yyvsp[-1].par_names );
			delete yyvsp[-1].par_names;
			yyval.id = yyvsp[-3].id;
		}
break;
case 21:
#line 155 "logo.y"
{
			yyval.par_names = new Array<String>;
		}
break;
case 22:
#line 159 "logo.y"
{
			yyval.par_names = yyvsp[0].par_names;
		}
break;
case 23:
#line 165 "logo.y"
{
			yyval.par_names = new Array<String>;
			yyval.par_names->append( yyvsp[0].id );
		}
break;
case 24:
#line 170 "logo.y"
{
			yyval.par_names = yyvsp[0].par_names;
			yyval.par_names->append( yyvsp[-2].id );
		}
break;
case 25:
#line 176 "logo.y"
{ block->push( new Sequence() ); }
break;
case 26:
#line 184 "logo.y"
{
			 expr->add( new EqualOperator() );
		}
break;
case 27:
#line 190 "logo.y"
{
			expr->add( Constant(yyvsp[0].id) );
		}
break;
case 28:
#line 199 "logo.y"
{ expr->add( Constant(yyvsp[0].number) ); }
break;
case 30:
#line 203 "logo.y"
{ expr->add( new PlusOperator() ); }
break;
case 31:
#line 205 "logo.y"
{ expr->add( new MinusOperator() ); }
break;
case 32:
#line 207 "logo.y"
{ expr->add( new MulOperator() ); }
break;
case 33:
#line 209 "logo.y"
{ expr->add( new DivOperator() ); }
break;
case 34:
#line 211 "logo.y"
{ expr->add( new ModOperator() ); }
break;
case 35:
#line 213 "logo.y"
{ expr->add( new IsEqualOperator() ); }
break;
case 36:
#line 215 "logo.y"
{ expr->add( new LessOperator() ); }
break;
case 37:
#line 217 "logo.y"
{ expr->add( new LessEqualOperator() ); }
break;
case 38:
#line 219 "logo.y"
{ expr->add( new GreaterOperator() ); }
break;
case 39:
#line 221 "logo.y"
{ expr->add( new GreaterEqualOperator() ); }
break;
case 40:
#line 223 "logo.y"
{ expr->add( new AndOperator() ); }
break;
case 41:
#line 225 "logo.y"
{ expr->add( new OrOperator() ); }
break;
case 43:
#line 229 "logo.y"
{ expr->add( new UnaryMinusOperator() ); }
break;
case 44:
#line 231 "logo.y"
{ expr->add( new UnaryPlusOperator() ); }
break;
case 46:
#line 240 "logo.y"
{	
			Array<String>	params;
			Ptr<Sequence>	body;

			try {
				proc_symbol_table->get( yyvsp[-3].id, params, body );
			}
			catch( const ProcSymbolTable::UNDEFINED & ) {
				char	buf[100];
				sprintf(buf, "function %s is not defined", yyvsp[-3].id);
				yyerror( buf );
				YYERROR;
			}
			if( params.num_elements() != yyvsp[-1].par_num ) {
				char	buf[100];
				sprintf( buf, "wrong number of parameters "
					"for %s", yyvsp[-3].id );
				yyerror( buf );
				YYERROR;
			}
			expr->add( new CallOperator(yyvsp[-3].id, yyvsp[-1].par_num) );
		}
break;
case 47:
#line 263 "logo.y"
{
			expr->add( new SinOperator() );
		}
break;
case 48:
#line 267 "logo.y"
{
			expr->add( new CosOperator() );
		}
break;
case 49:
#line 271 "logo.y"
{
			expr->add( new TanOperator() );
		}
break;
case 50:
#line 275 "logo.y"
{
			expr->add( new ACosOperator() );
		}
break;
case 51:
#line 279 "logo.y"
{
			expr->add( new ATanOperator() );
		}
break;
case 52:
#line 284 "logo.y"
{
			yyval.par_num = 0;
		}
break;
case 53:
#line 288 "logo.y"
{
			yyval.par_num = yyvsp[0].par_num;
		}
break;
case 54:
#line 294 "logo.y"
{
			yyval.par_num = 1;
		}
break;
case 55:
#line 298 "logo.y"
{
			yyval.par_num = yyvsp[-2].par_num + 1;
		}
break;
case 56:
#line 308 "logo.y"
{ expr->add( new PrintOperator() ); }
break;
case 57:
#line 310 "logo.y"
{ expr->add( new ForwardOperator() ); }
break;
case 58:
#line 312 "logo.y"
{ expr->add( new BackwardOperator() ); }
break;
case 59:
#line 314 "logo.y"
{ expr->add( new LeftOperator() ); }
break;
case 60:
#line 316 "logo.y"
{ expr->add( new RightOperator() ); }
break;
case 61:
#line 318 "logo.y"
{ expr->add( new PenUpOperator() ); }
break;
case 62:
#line 320 "logo.y"
{ expr->add( new PenDownOperator() ); }
break;
case 63:
#line 322 "logo.y"
{ expr->add( new DirectionOperator() ); }
break;
case 64:
#line 324 "logo.y"
{ expr->add( new PositionOperator() ); }
break;
#line 827 "y.tab.c"
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
