
%{

#define	MAX_ID_LEN	30

#include	<stdlib.h>
#include	<string.h>
#include	<cstring.h>
#include	"stdtypes.h"
#include	"y.tab.h"

int	line_no;

%}

DIGIT	[0-9]
ID	[a-zA-Z_][a-zA-Z_0-9_]*

%%

{DIGIT}+|{DIGIT}+"."{DIGIT}*	{
		yylval.number = atof(yytext);
		return NUMBER;
	}

while	{ return WHILE; }

if	{ return IF; }

then	{ return THEN; }

else	{ return ELSE; }

function { return FUNCTION; }

end	{ return END; }

sin	{ return SIN; }

cos	{ return COS; }

tan	{ return TAN; }

atan	{ return ATAN; }

acos	{ return ACOS; }

"<="	{ return LE; }

">="	{ return GE; }

print		{ return PRINT; }
forward		{ return FORWARD; }
backward	{ return BACKWARD; }
left		{ return LEFT; }
right		{ return RIGHT; }
pen		{ return PEN; }
up		{ return UP; }
down		{ return DOWN; }
direction	{ return DIRECTION; }
position	{ return POSITION; }

{ID}	{
	strncpy( yylval.id, yytext, MAX_ID_LEN );
	yylval.id[MAX_ID_LEN] = '\0';
	return ID;
	}

[ \t]+

"#".*"\n"	{ line_no++; /* comment */ }

"\n"	{ line_no++; return yytext[0]; }

.	{ return yytext[0]; }

%%
