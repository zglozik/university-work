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
typedef union {
	Expression	*expr;
	Boundaries	boundaries;
	_Point		point;
	PartPoint	part_point;
	double		number;
	bool		route;
	char		id[MAX_ID_LEN+1];
} YYSTYPE;
extern YYSTYPE yylval;
