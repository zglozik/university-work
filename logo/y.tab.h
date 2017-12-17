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
typedef union{
	char		id[MAX_ID_LEN+1];
	double		number;
	Array<String>	*par_names;
	int		par_num;
} YYSTYPE;
extern YYSTYPE yylval;
