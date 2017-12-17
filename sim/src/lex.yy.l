
%{

#include	"bodyspec.hh"
#include	"y.tab.h"
#include	<string.h>

%}

COMMENT	#.*\n
DIGIT	[0-9]
ID	[a-zA-Z][a-zA-Z0-9_]*

%%

{COMMENT}	{ yyline_no++; }

"BODY"		{ return BODY; }
"PARTS"		{ return PARTS; }
"WRISTS"	{ return WRISTS; }
"INIT"		{ return INIT; }
"PROGRAM"	{ return PROGRAM; }
"fixed"		{ return FIXED; }
"start"		{ return START; }
"end"		{ return END; }

"sqrt"		{ return SQRT; }
"sin"		{ return SIN; }
"cos"		{ return COS; }
"tan"		{ return TAN; }
"exp"		{ return EXP; }

"ROUTE"		{ yylval.route = true; return ROUTE; }
"NOROUTE"	{ yylval.route = false; return ROUTE; }

{DIGIT}+|{DIGIT}+"."{DIGIT}*	{
	yylval.number = atof(yytext);
	return NUMBER;
	}

{ID}	{
	strncpy( yylval.id, yytext, MAX_ID_LEN );
	yylval.id[ MAX_ID_LEN ] = '\0';
	return ID;
	}

[ \t]+	{ }

\n	{ yyline_no++; }

.	{ return yytext[0]; }


%%

