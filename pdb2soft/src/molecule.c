
/*------------------------------------------------------------
 *	molecule.c -- routines to build molecule database from file
 *------------------------------------------------------------*/

#include	"molecule.h"
#include	<string.h>
#include	<stdlib.h>

/* important PDB record types */

typedef enum {
ATOM, HETATM, CONECT, TER, ENDMDL, END 
} RecordType;

/*------------------------------------------------------------
 *	Function prototypes
 *------------------------------------------------------------*/

static char	*GetString( char *record, char *buf, int i, int j );
static double 	GetDouble( char *record, int i, int j );
static int 	GetInteger( char *record, int i, int j );
static char	GetChar( char *record, int i );

static bool ReadLine( FILE *file, char *buf );
static bool ReadRecord( FILE *file, char *record, RecordType *rt );
static void ProcessRecord( Molecule *m, char *record, RecordType rt );

static void ChangeChain( Molecule *m, int chain_id );
static void ChangeGroup( Molecule *m, int seqno, char *res_name );
static void InsertBond( Molecule *m, Atom *a, Atom *b, int flag );
static void InsertBondSerial( Molecule *m, int a, int b );

static void xyz2ijk( Molecule *m, double width[],
		    double x, double y, double z,
		    int	*i, int *j, int *k );
static void TestBonded( Molecule *m, Atom *src, Atom *dst );
static void CreateMoleculeBonds( Molecule *m );
static void FreeBonds( Molecule *m, int flag );

static void ProcessAtom( Molecule *m, char *name, char *res_name,
			double dx, double  dy, double dz,
			int serno, int seqno,
			int chain_id, int ins_code );

static int GetElemNumber( Atom *atom );

/*------------------------------------------------------------
 *	Constants and macros
 *------------------------------------------------------------*/

#define	FOR_EACH_ATOM( c, g, a )			\
for( c = m->chains; c; c = c->next )			\
	for( g = c->groups; g; g = g->next )		\
		for( a = g->atoms; a; a = a->next )

#define	LENGTH2( a, b, c )	((a)*(a) + (b)*(b) + (c)*(c))

#define	MAX( a, b )		((a) > (b) ? (a) : (b))
#define	MIN( a, b )		((a) > (b) ? (b) : (a))

#define IS_ALPHA_CARBON(x)	((x)==1)
#define IS_SUGAR_PHOSPHATE(x)	((x)==7)
#define IS_NUCLEO(x)		(((x)>=24) && ((x)<=27))
#define IS_PROTEIN(x)		(((x)<=23) || (((x)>=28) && ((x)<=30)))

/* these are in Armstrong coodinates */
#define	MAX_BOND_DIST	(475.0 / 250.0)
#define	MIN_BOND_DIST	(100.0 / 250.0)
#define	MAX_HBOND_DIST	(300.0 / 250.0)

/* colors in CPK */

/*  0 - light grey */
/*  1 - sky blue */
/*  2 - red */
/*  3 - yellow */
/*  4 - white */
/*  5 - pink */
/*  6 - golden rod */
/*  7 - blue */
/*  8 - orange */
/*  9 - dark grey */
/* 10 - brown */
/* 11 - purple */
/* 12 - deep pink */
/* 13 - green */
/* 14 - fire brick */
/* 15 - mid green */
/* 16 - white */
/* 17 - gray */
/* 18 - red */
/* 19 - mid blue */
/* 20 - yellow */
/* 21 - mid green */

/* if specular or diffuse color is -1, it means that the correct value must be
   calculated using the ambient color. */

Color	colors[MAXCOLOR_NO] =
{
{ -1, -1, -1, 255./211, 255./211, 255./211, -1, -1, -1 },
{ -1, -1, -1, 255./135, 255./206, 255./235, -1, -1, -1 },
{ -1, -1, -1, 255./255,        0,        0, -1, -1, -1 },
{ -1, -1, -1, 255./255, 255./255,        0, -1, -1, -1 },
{ -1, -1, -1, 255./255, 255./255, 255./255, -1, -1, -1 },
{ -1, -1, -1, 255./255, 255./192, 255./203, -1, -1, -1 },
{ -1, -1, -1, 255./218, 255./165,  255./32, -1, -1, -1 },
{ -1, -1, -1,        0,        0, 255./255, -1, -1, -1 },
{ -1, -1, -1, 255./255, 255./165,        0, -1, -1, -1 },
{ -1, -1, -1, 255./169, 255./169, 255./169, -1, -1, -1 },
{ -1, -1, -1, 255./165,  255./42,  255./42, -1, -1, -1 },
{ -1, -1, -1, 255./160,  255./32, 255./240, -1, -1, -1 },
{ -1, -1, -1, 255./255,  255./20, 255./147, -1, -1, -1 },
{ -1, -1, -1,        0, 255./255,        0, -1, -1, -1 },
{ -1, -1, -1, 255./178,  255./34,  255./34, -1, -1, -1 },
{ -1, -1, -1,        0, 255./205,        0, -1, -1, -1 },
{  1,  1,  1, 0.9, 0.9, 0.9, 1, 1, 1 },
{ 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5 },
{ 1, 0.6, 0.6, 1, 0, 0, 1, 0, 0 },
{ 0.6, 0.6, 1, 0, 0, 0.7, 0.2, 0.2, 1 },
{ 1, 1, 0.6, 0.7, 0.7, 0, 1, 1, 0 },
{ 0.133, 0.557, 0.133, 0, 0.25, 0, 0.133, 0.557, 0.133 }
};

ElemStruct elements[MAXELEM_NO] =  {
    { { ' ', ' ' }, 170, 12, 1,		""             },  /*   0 */
    { { 'H', ' ' },  80, 16, 1,		"HYDROGEN"     },  /*   1 */
    { { 'H', 'e' }, 400,  5, 1,		"HELIUM"       },  /*   2 */
    { { 'L', 'i' }, 170, 14, 1,		"LITHIUM"      },  /*   3 */
    { { 'B', 'e' },  88, 12, 1,		"BERYLLIUM"    },  /*   4 */
    { { 'B', ' ' }, 205, 13, 1,		"BORON"        },  /*   5 */
    { { 'C', ' ' }, 170, 17, 1.5417,	"CARBON"       },  /*   6 */
    { { 'N', ' ' }, 188, 19, 1.2833,	"NITROGEN"     },  /*   7 */
    { { 'O', ' ' }, 182, 18, 1.1667,	"OXYGEN"       },  /*   8 */
    { { 'F', ' ' }, 180,  6, 1,		"FLUORINE"     },  /*   9 */
    { { 'N', 'e' }, 280, 12, 1,		"NEON"         },  /*  10 */
    { { 'N', 'a' }, 243,  7, 1,		"SODIUM"       },  /*  11 */
    { { 'M', 'g' }, 275, 15, 1,		"MAGNESIUM"    },  /*  12 */
    { { 'A', 'l' }, 338,  9, 1,		"ALUMINIUM"    },  /*  13 */
    { { 'S', 'i' }, 300,  6, 1,		"SILICON"      },  /*  14 */
    { { 'P', ' ' }, 262, 21, 1.5833,	"PHOSPHORUS"   },  /*  15 */
    { { 'S', ' ' }, 255, 20, 1.5417,	"SULPHUR"      },  /*  16 */
    { { 'C', 'l' }, 248, 13, 1,		"CHLORINE"     },  /*  17 */
    { { 'A', 'r' }, 392, 12, 1,		"ARGON"        },  /*  18 */
    { { 'K', ' ' }, 332, 12, 1,		"POTASSIUM"    },  /*  19 */
    { { 'C', 'a' }, 248,  9, 1,		"CALCIUM"      },  /*  20 */
    { { 'S', 'c' }, 360, 12, 1,		"SCANDIUM"     },  /*  21 */
    { { 'T', 'i' }, 368,  9, 1,		"TITANIUM"     },  /*  22 */
    { { 'V', ' ' }, 332, 12, 1,		"VANADIUM"     },  /*  23 */
    { { 'C', 'r' }, 338,  9, 1,		"CHROMIUM"     },  /*  24 */
    { { 'M', 'n' }, 338,  9, 1,		"MANGANESE"    },  /*  25 */
    { { 'F', 'e' }, 335,  8, 1,		"IRON"         },  /*  26 */
    { { 'C', 'o' }, 332, 12, 1,		"COBALT"       },  /*  27 */
    { { 'N', 'i' }, 375, 10, 1,		"NICKEL"       },  /*  28 */
    { { 'C', 'u' }, 380, 10, 1,		"COPPER"       },  /*  29 */
    { { 'Z', 'n' }, 362, 10, 1,		"ZINC"         },  /*  30 */
    { { 'G', 'a' }, 305, 12, 1,		"GALLIUM"      },  /*  31 */
    { { 'G', 'e' }, 292, 12, 1,		"GERMANIUM"    },  /*  32 */  /*1225?*/
    { { 'A', 's' }, 302, 12, 1,		"ARSENIC"      },  /*  33 */
    { { 'S', 'e' }, 305, 12, 1,		"SELENIUM"     },  /*  34 */
    { { 'B', 'r' }, 302, 10, 1,		"BROMINE"      },  /*  35 */
    { { 'K', 'r' }, 400, 12, 1,		"KRYPTON"      },  /*  36 */
    { { 'R', 'b' }, 368, 12, 1,		"RUBIDIUM"     },  /*  37 */
    { { 'S', 'r' }, 280, 12, 1,		"STRONTIUM"    },  /*  38 */
    { { 'Y', ' ' }, 445, 12, 1,		"YTTRIUM"      },  /*  39 */
    { { 'Z', 'r' }, 390, 12, 1,		"ZIRCONIUM"    },  /*  40 */
    { { 'N', 'b' }, 370, 12, 1,		"NIOBIUM"      },  /*  41 */
    { { 'M', 'o' }, 368, 12, 1,		"MOLYBDENUM"   },  /*  42 */
    { { 'T', 'c' }, 338, 12, 1,		"TECHNETIUM"   },  /*  43 */
    { { 'R', 'u' }, 350, 12, 1,		"RUTHENIUM"    },  /*  44 */
    { { 'R', 'h' }, 362, 12, 1,		"RHODIUM"      },  /*  45 */
    { { 'P', 'd' }, 375, 12, 1,		"PALLADIUM"    },  /*  46 */
    { { 'A', 'g' }, 398,  9, 1,		"SILVER"       },  /*  47 */
    { { 'C', 'd' }, 422, 12, 1,		"CADMIUM"      },  /*  48 */
    { { 'I', 'n' }, 407, 12, 1,		"INDIUM"       },  /*  49 */
    { { 'S', 'n' }, 365, 12, 1,		"TIN",         },  /*  50 */
    { { 'S', 'b' }, 155, 12, 1,		"ANTIMONY"     },  /*  51 */
    { { 'T', 'e' }, 175, 12, 1,		"TELLURIUM"    },  /*  52 */
    { { 'I', ' ' }, 350, 11, 1,		"IODINE"       },  /*  53 */
    { { 'X', 'e' }, 425, 12, 1,		"XENON"        },  /*  54 */
    { { 'C', 's' }, 418, 12, 1,		"CAESIUM"      },  /*  55 */
    { { 'B', 'a' }, 335,  8, 1,		"BARIUM"       },  /*  56 */
    { { 'L', 'a' }, 255, 12, 1,		"LANTHANUM"    },  /*  57 */
    { { 'C', 'e' }, 258, 12, 1,		"CERIUM"       },  /*  58 */
    { { 'P', 'r' }, 225, 12, 1,		"PRASEODYMIUM" },  /*  59 */
    { { 'N', 'd' }, 248, 12, 1,		"NEODYMIUM"    },  /*  60 */
    { { 'P', 'm' }, 245, 12, 1,		"PROMETHIUM"   },  /*  61 */
    { { 'S', 'm' }, 240, 12, 1,		"SAMARIUM"     },  /*  62 */
    { { 'E', 'u' }, 273, 12, 1,		"EUROPIUM"     },  /*  63 */
    { { 'G', 'd' }, 235, 12, 1,		"GADOLINIUM"   },  /*  64 */
    { { 'T', 'b' }, 230, 12, 1,		"TERBIUM"      },  /*  65 */
    { { 'D', 'y' }, 228, 12, 1,		"DYSPROSIUM"   },  /*  66 */
    { { 'H', 'o' }, 222, 12, 1,		"HOLMIUM"      },  /*  67 */
    { { 'E', 'r' }, 220, 12, 1,		"ERBIUM"       },  /*  68 */
    { { 'T', 'm' }, 218, 12, 1,		"THULIUM"      },  /*  69 */
    { { 'Y', 'b' }, 215, 12, 1,		"YTTERBIUM"    },  /*  70 */
    { { 'L', 'u' }, 212, 12, 1,		"LUTETIUM"     },  /*  71 */
    { { 'H', 'f' }, 195, 12, 1,		"HAFNIUM"      },  /*  72 */
    { { 'T', 'a' }, 170, 12, 1,		"TANTALUM"     },  /*  73 */
    { { 'W', ' ' }, 175, 12, 1,		"TUNGSTEN"     },  /*  74 */
    { { 'R', 'e' }, 180, 12, 1,		"RHENIUM"      },  /*  75 */
    { { 'O', 's' }, 220, 12, 1,		"OSMIUM"       },  /*  76 */
    { { 'I', 'r' }, 170, 12, 1,		"IRIDIUM"      },  /*  77 */
    { { 'P', 't' }, 325, 12, 1,		"PLATINUM"     },  /*  78 */
    { { 'A', 'u' }, 335,  6, 1,		"GOLD"         },  /*  79 */
    { { 'H', 'g' }, 275, 12, 1,		"MERCURY"      },  /*  80 */
    { { 'T', 'l' }, 238, 12, 1,		"THALLIUM"     },  /*  81 */
    { { 'P', 'b' }, 300, 12, 1,		"LEAD"         },  /*  82 */
    { { 'B', 'i' }, 240, 12, 1,		"BISMUTH"      },  /*  83 */
    { { 'P', 'o' }, 168, 12, 1,		"POLONIUM"     },  /*  84 */
    { { 'A', 't' }, 155, 12, 1,		"ASTATINE"     },  /*  85 */
    { { 'R', 'n' }, 475, 12, 1,		"RADON"        },  /*  86 */
    { { 'F', 'r' }, 450, 12, 1,		"FRANCIUM"     },  /*  87 */
    { { 'R', 'a' }, 358, 12, 1,		"RADIUM"       },  /*  88 */
    { { 'A', 'c' }, 295, 12, 1,		"ACTINIUM"     },  /*  89 */
    { { 'T', 'h' }, 255, 12, 1,		"THORIUM"      },  /*  90 */
    { { 'P', 'a' }, 222, 12, 1,		"PROTACTINIUM" },  /*  91 */
    { { 'U', ' ' }, 242, 12, 1,		"URANIUM"      },  /*  92 */
    { { 'N', 'p' }, 238, 12, 1,		"NEPTUNIUM"    },  /*  93 */
    { { 'P', 'u' }, 232, 12, 1,		"PLUTONIUM"    },  /*  94 */
    { { 'A', 'm' }, 230, 12, 1,		"AMERICIUM"    },  /*  95 */
    { { 'C', 'm' }, 228, 12, 1,		"CURIUM"       },  /*  96 */
    { { 'B', 'k' }, 225, 12, 1,		"BERKELIUM"    },  /*  97 */
    { { 'C', 'f' }, 222, 12, 1,		"CALIFORNIUM"  },  /*  98 */
    { { 'E', 's' }, 220, 12, 1,		"EINSTEINIUM"  },  /*  99 */
    { { 'F', 'm' }, 218, 12, 1,		"FERMIUM"      },  /* 100 */
    { { 'M', 'd' }, 215, 12, 1,		"MENDELEVIUM"  },  /* 101 */
    { { 'N', 'o' }, 212, 12, 1,		"NOBELIUM"     },  /* 102 */
    { { 'L', 'r' }, 210, 12, 1,		"LAWRENCIUM"   }   /* 103 */ /* Lw? */
    };

#define	MAX_ELEM	256
#define	ELEM_NAME_LEN	4

static int	elem_number = 29; /* number of elements in array elem_desc */

static char elem_desc[MAX_ELEM][ELEM_NAME_LEN] = 
{
{ ' ', 'N', ' ', ' ' },  /* 0*/
{ ' ', 'C', 'A', ' ' },  /* 1*/
{ ' ', 'C', ' ', ' ' },  /* 2*/
{ ' ', 'O', ' ', ' ' },  /* 3*/   /* 0-3   Amino Acid Backbone    */
{ ' ', 'C', '\'', ' ' }, /* 4*/
{ ' ', 'O', 'T', ' ' },  /* 5*/
{ ' ', 'S', ' ', ' ' },  /* 6*/
{ ' ', 'P', ' ', ' ' },  /* 7*/   /* 4-7   Shapely Amino Backbone */
{ ' ', 'O', '1', 'P' },  /* 8*/
{ ' ', 'O', '2', 'P' },  /* 9*/
{ ' ', 'O', '5', '*' },  /*10*/
{ ' ', 'C', '5', '*' },  /*11*/
{ ' ', 'C', '4', '*' },  /*12*/
{ ' ', 'O', '4', '*' },  /*13*/
{ ' ', 'C', '3', '*' },  /*14*/
{ ' ', 'O', '3', '*' },  /*15*/
{ ' ', 'C', '2', '*' },  /*16*/
{ ' ', 'O', '2', '*' },  /*17*/
{ ' ', 'C', '1', '*' },  /*18*/   /* 7-18  Nucleic Acid Backbone  */
{ ' ', 'C', 'A', '2' },  /*19*/   /* 19    Shapely Special        */
{ ' ', 'S', 'G', ' ' },  /*20*/   /* 20    Cysteine Sulphur       */
{ ' ', 'N', '1', ' ' },  /*21*/
{ ' ', 'N', '2', ' ' },  /*22*/
{ ' ', 'N', '3', ' ' },  /*23*/
{ ' ', 'N', '4', ' ' },  /*24*/
{ ' ', 'N', '6', ' ' },  /*25*/
{ ' ', 'O', '2', ' ' },  /*26*/
{ ' ', 'O', '4', ' ' },  /*27*/
{ ' ', 'O', '6', ' ' }   /*28*/   /* 21-28 Nucleic Acid H-Bonding */
};

#define MAX_RES  		80
#define	RESIDUE_NAME_LEN	3

static int residue_number = 35;	/* number of elements in residue array */

static char *residue_desc[MAX_RES] = {
/*===============*/
/*  Amino Acids  */
/*===============*/

/* Ordered by Cumulative Frequency in Brookhaven *
 * Protein Databank, December 1991               */

"ALA", /* 8.4% */     "GLY", /* 8.3% */
"LEU", /* 8.0% */     "SER", /* 7.5% */
"VAL", /* 7.1% */     "THR", /* 6.4% */
"LYS", /* 5.8% */     "ASP", /* 5.5% */
"ILE", /* 5.2% */     "ASN", /* 4.9% */
"GLU", /* 4.9% */     "PRO", /* 4.4% */
"ARG", /* 3.8% */     "PHE", /* 3.7% */
"GLN", /* 3.5% */     "TYR", /* 3.5% */
"HIS", /* 2.3% */     "CYS", /* 2.0% */
"MET", /* 1.8% */     "TRP", /* 1.4% */

"ASX", "GLX", "PCA", "HYP",

/*===============*/
/*  Nucleotides  */
/*===============*/
"  A", "  C", "  G", "  T",

/*=================*/
/*  Miscellaneous  */ 
/*=================*/

"UNK", "ACE", "FOR", "HOH",
"DOD", "SO4", "PO4"
};

/* names of record types */

static char	*rec_names[] =
{
	"ATOM  ",
	"HETATM",
	"CONECT",
	"TER   ",
	"ENDMDL",
	"END   "
};

#define	LINE_LEN	80

/*------------------------------------------------------------
 *	Function definitions
 *------------------------------------------------------------*/

/*----------------------------------------------------------------------
 *extern double GetRadius( Atom *a );
 *----------------------------------------------------------------------*/

extern double GetRadius( Atom *a )
{
	return elements[GetElemNumber(a)].radius;
}

/*----------------------------------------------------------------------
 *extern char	*GetName( Atom *a );
 *----------------------------------------------------------------------*/

extern char *GetName( Atom *a )
{
	return elements[GetElemNumber(a)].name;
}

/*------------------------------------------------------------
 *extern Color	*GetColor( Atom *a );
 *------------------------------------------------------------*/

extern Color *GetColor( Atom *a )
{
	return &colors[ elements[GetElemNumber(a)].cpkcol ];
}

/*------------------------------------------------------------
 *extern Molecule	*MlclCreate( FILE *file );
 *------------------------------------------------------------*/

extern Molecule	*MlclCreate( FILE *file )
{
	Molecule	*mlcl;
	char		record[LINE_LEN+1];
	bool		eof;
	RecordType	rt;

	mlcl = ALLOC( 1, Molecule );
	mlcl->bonds = NULL;
	mlcl->chains = NULL;
	mlcl->last_atom = NULL;
	mlcl->main_atoms_no = 0;
	mlcl->heta_atoms_no = 0;
	mlcl->bonds_no = 0;
	mlcl->change_chain = true;
	mlcl->first_atom = true;

	eof = ReadRecord( file, record, &rt );
	while( !eof && rt != ENDMDL ) {
		ProcessRecord( mlcl, record, rt );
		eof = ReadRecord( file, record, &rt );
	}
	INFO(( info_file, "--------------, bonds: %d, atoms: %d\n", 
		mlcl->bonds_no, mlcl->main_atoms_no ));

	if( mlcl->bonds_no <= mlcl->main_atoms_no ) {
		mlcl->fast_bond_calc = 
			mlcl->main_atoms_no + mlcl->heta_atoms_no > 255;
		CreateMoleculeBonds( mlcl );
	}

	return mlcl;
}

/*------------------------------------------------------------
 *extern void MlclFree( Molecule *m );
 *------------------------------------------------------------*/

extern void MlclFree( Molecule *m )
{
	while( m->bonds ) {
		Bond	*tmp;

		tmp = m->bonds;
		m->bonds = m->bonds->next;
		FREE( tmp );
	}
	while( m->chains ) {
		Chain	*tc;

		tc = m->chains;
		m->chains = m->chains->next;
		while( tc->groups ) {
			Group	*tg;

			tg = tc->groups;
			tc->groups = tc->groups->next;
			while( tg->atoms ) {
				Atom	*ta;

				ta = tg->atoms;
				tg->atoms = tg->atoms->next;
				FREE( ta );
			}
			FREE( tg );
		}
		FREE( tc );
	}
	FREE( m );
}

/*------------------------------------------------------------
 *static bool ReadLine( FILE *file, char *buf );
 *------------------------------------------------------------*/

static bool ReadLine( FILE *file, char *buf )
{
	int	c;
	char	*p = buf;

	c = fgetc( file );
	while( c != EOF && c != '\n' && c != '\r' && p-buf < LINE_LEN ) {
		*p++ = c;
		c = fgetc( file );
	}
	*p = '\0';
	while( c != EOF && c != '\n' && c != '\r' )
		c = fgetc( file );
	
	return c == EOF;
}
	
/*------------------------------------------------------------
 *static bool ReadRecord( file, record, rt );
 *------------------------------------------------------------*/

static bool ReadRecord( file, record, rt )
	FILE		*file;
	char		*record;
	RecordType	*rt;
{
	bool	eof;
	bool	found;
	int	i;

	eof = ReadLine( file, record );
	found = false;
	for( i = 0; i < ARRAY_SIZE(rec_names) && !found; i++ )
		found = !strncmp( record, rec_names[i], 6 );
	while( !eof && !found ) {
		eof = ReadLine( file, record );
		for( i = 0; i < ARRAY_SIZE(rec_names) && !found; i++ )
			found = !strncmp( record, rec_names[i], 6 );
	}
	*rt = i-1;
	return eof;
}

/*------------------------------------------------------------
 *static void ProcessRecord( Molecule *m, char *record, RecordType rt );
 *------------------------------------------------------------*/

static void ProcessRecord( Molecule *m, char *record, RecordType rt )
{
	char	name[5];
	char	res_name[4];
	double	dx, dy, dz;
	int	serno, seqno;
	int	chain_id, ins_code;
	int	i;
	int	src, dst;

	switch( rt ) {
	    case ATOM:
	    case HETATM:
		if( rt == ATOM ) {
			m->main_atoms_no++;
		} else {
			m->heta_atoms_no++;
		}

		GetString( record, name, 13, 16 );
		GetString( record, res_name, 18, 20 );
			  
		dx = GetDouble( record, 31, 38 );
		dy = GetDouble( record, 39, 46 );
		dz = GetDouble( record, 47, 54 );
		serno = GetInteger( record, 7, 11 );
		seqno = GetInteger( record, 23, 26 );
		chain_id = GetChar( record, 22 );
		ins_code = GetChar( record, 27 );
		ProcessAtom( m, name, res_name, dx, dy, dz, serno, seqno,
			    chain_id, ins_code );
		break;
	    case CONECT:
		src = GetInteger( record, 7, 11 );
		for( i = 0; i < 10; i++ ) {
			dst = GetInteger( record, 12 + i*5, 16 + i*5 );
			if( dst > src ) {
				m->bonds_no++;
				InsertBondSerial( m, src, dst );
			}
		}
		break;
	    case TER:
		m->change_chain = true;
		break;
	    case END:
		m->change_chain = true;
		break;
	    default:
		fprintf( stderr, "bad record type!\n" );
		exit( 1 );
	}
}

/*------------------------------------------------------------
 *static void ChangeGroup( Molecule *m, int seqno, char *res_name );
 *------------------------------------------------------------*/

static void ChangeGroup( Molecule *m, int seqno, char *res_name )
{
	Group	*g;
	int	i;
	bool	found;

	TEST_EXPR( m->chains != NULL );

	g = ALLOC( 1, Group );
	g->atoms = NULL;
	g->seqno = seqno;

	found = false;
	for( i = 0; i < residue_number && !found; i++ ) {
		found = !strncmp( res_name, residue_desc[i],
				 RESIDUE_NAME_LEN );
	}
	if( !found ) {
		if( !strncmp( res_name, "CSH", 3) ||
		   !strncmp( res_name, "CYH", 3) ||
		   !strncmp( res_name, "CSM", 3) ) {
			g->resno = 17; /* cystine */
		} else if( !strncmp(res_name,"WAT",3) ||
			  !strncmp(res_name,"H2O",3) ||
			  !strncmp(res_name,"SOL",3) ||
			  !strncmp(res_name,"TIP",3) ) {
			g->resno = 31; /* Water (HOH) */
		} else if( !strncmp(res_name,"D2O",3) )	{
			g->resno = 32; /* Heavy water (DOD) */
		} else if( !strncmp(res_name,"SUL",3) )	{
			g->resno = 33; /* Sulphate (SO4) */
		} else if( !strncmp(res_name,"CPR",3) )	{
			g->resno = 11; /* cis-proline */
		} else if( !strncmp(res_name,"TRY",3) ) {
			g->resno = 15; /* tryptophan */
		} else if( residue_number == MAX_RES ) {
			fprintf( stderr, "too many residues, sorry\n" );
			exit( 1 );
		} else {
			g->resno = residue_number;
			residue_desc[residue_number++] = strdup(res_name);
		}
	} else {
		g->resno = i-1;
	}
	g->next = m->chains->groups;
	m->chains->groups = g;
}

/*------------------------------------------------------------
 *static void ChangeChain( Molecule *m, int chain_id );
 *------------------------------------------------------------*/

static void ChangeChain( Molecule *m, int chain_id )
{
	Chain	*c;

	m->change_chain = false;
	c = ALLOC( 1, Chain );
	c->groups = NULL;
	c->ident = chain_id;
	c->next = m->chains;
	m->chains = c;
	m->last_atom = NULL;	/* break at chain breaks */
}

/*------------------------------------------------------------
 *static void InsertBond( Molecule *m, Atom *a, Atom *b, int flag );
 *------------------------------------------------------------*/

static void InsertBond( Molecule *m, Atom *a, Atom *b, int flag )
{
	Bond	*bond = ALLOC( 1, Bond );

	bond->src = a;
	bond->dst = b;
	bond->flag = flag;
	bond->next = m->bonds;
	m->bonds = bond;
#ifdef DEBUG
	fprintf( info_file, "bond: src: %4d, dst: %4d, flag: %d\n", 
	MIN(a->serno, b->serno), MAX(a->serno, b->serno), flag );
#endif
}

/*------------------------------------------------------------
 *static void InsertBondSerial( Molecule *m, int a, int b );
 *------------------------------------------------------------*/

static void InsertBondSerial( Molecule *m, int a, int b )
{
	Atom	*src, *dst;
	Chain	*c;
	Group	*g;
	Atom	*p;

	src = NULL;
	dst = NULL;
	for( c = m->chains; c && (!src || !dst); c = c->next ) {
		for( g = c->groups; g && (!src || !dst); g = g->next ) {
			for( p = g->atoms; p && (!src || !dst); p = p->next ) {
				if( p->serno == a ) {
					src = p;
				} else if( p->serno == b ) {
					dst = p;
				}
			}
		}
	}
	if( src && dst )
		InsertBond( m, src, dst, BOND_CONECT );
}

/*------------------------------------------------------------
 *static void ProcessAtom( m, name, res_name, dx, dy, dz,
 *			  serno, seqno, chain_id, ins_code );
 *------------------------------------------------------------*/

static void ProcessAtom( m, name, res_name, dx, dy, dz,
			serno, seqno, chain_id, ins_code )
	Molecule	*m;
	char		*res_name;
	char		*name;
	double		dx, dy, dz;
	int		serno, seqno;
	int		chain_id;
	int		ins_code;
{
	Atom	*a = ALLOC( 1, Atom );
	bool	found;
	int	i;
	double	coordinates[3];

	coordinates[0] = dx;
	coordinates[1] = dy;
	coordinates[2] = dz;

	for( i = 0; i < 3; i++ ) {
		if( coordinates[i] < m->min[i] || m->first_atom ) 
			m->min[i] = coordinates[i];
		if( coordinates[i] > m->max[i] || m->first_atom )
			m->max[i] = coordinates[i];
	}
	m->first_atom = false;

	a->x = dx;
	a->y = dy;
	a->z = dz;
	a->r = 1;
	a->serno = serno;
	a->roommate = NULL;
	a->hydrogen = GetElemNumber( a ) == 1;

	if( m->change_chain || !m->chains || m->chains->ident != chain_id ) {
		ChangeChain( m, chain_id );
	}
	if( !m->chains->groups || m->chains->groups->seqno != seqno || 
	   (m->chains->groups->atoms &&
	    m->chains->groups->atoms->ins_code != ins_code) ) {
		ChangeGroup( m, seqno, res_name );
	}

	UpperStr( name );
	found = false;
	for( i = 0; i < elem_number && !found; i++ )
		found = !strncmp( elem_desc[i], name, ELEM_NAME_LEN );
	
	if( !found ) {
		if( elem_number == MAX_ELEM ) {
			fprintf( stderr, "too many new elem, sorry\n" );
			exit( 1 );
		}
		a->ref_no = elem_number;
		memcpy( elem_desc[elem_number++], name, ELEM_NAME_LEN );
	} else {
		a->ref_no = i-1;
	}

#ifdef	DEBUG
	fprintf( info_file, "atom: %s, residue: %s, ref_no: %d,"
		" x: %f, y: %f, z: %f\n", 
		name, res_name, a->ref_no, dx, dy, dz );
#endif

	if( IS_ALPHA_CARBON( a->ref_no ) && 
	   IS_PROTEIN( m->chains->groups->resno ) ) {
		if( m->last_atom ) {
			dx = m->last_atom->x - a->x;
			dy = m->last_atom->y - a->y;
			dz = m->last_atom->z - a->z;

			if( LENGTH2( dx, dy, dz ) < 7*7 ) {
				InsertBond(m, m->last_atom, a, BOND_IMPLICIT);
			}
		}
		m->last_atom = a;
	} else if( IS_SUGAR_PHOSPHATE(a->ref_no) && 
		  IS_NUCLEO(m->chains->groups->resno) ) {
		if( m->last_atom ) {
			InsertBond( m, m->last_atom, a, BOND_IMPLICIT );
		}
		m->last_atom = a;
	}
	
	a->next = m->chains->groups->atoms;
	m->chains->groups->atoms = a;
}

/*------------------------------------------------------------
 *static char *GetString( char *record, char *buf, int i, int j );
 *------------------------------------------------------------*/

static char *GetString( char *record, char *buf, int i, int j )
{
	int	l;

	if( strlen(record) < i ) return "";

	for( l = i-1; l < j && record[l]; l++ )
		buf[l-i+1] = record[l];
	buf[l-i+1] = '\0';

	return buf;
}

/*------------------------------------------------------------
 *static double GetDouble( char *record, int i, int j );
 *------------------------------------------------------------*/

static double GetDouble( char *record, int i, int j )
{
	char	buf[LINE_LEN+1];

	if( strlen(record) < i ) return 0;

	strncpy( buf, &record[i-1], j-i+1 );
	buf[j-i+1] = '\0';
	
	return atof(buf);
}

/*------------------------------------------------------------
 *static int GetInteger( char *record, int i, int j );
 *------------------------------------------------------------*/

static int GetInteger( char *record, int i, int j )
{
	char	buf[LINE_LEN+1];

	if( strlen(record) < i ) return 0;

	strncpy( buf, &record[i-1], j-i+1 );
	buf[j-i+1] = '\0';
	
	return atoi(buf);
}

/*------------------------------------------------------------
 *static char GetChar( char *record, int i );
 *------------------------------------------------------------*/

static char GetChar( char *record, int i )
{
	if( strlen(record) < i ) return '\0';

	return record[i-1];
}

/*------------------------------------------------------------
 *static void FreeBonds( Molecule *m, int flag );
 *------------------------------------------------------------*/

static void FreeBonds( Molecule *m, int flag )
{
	Bond	*prev = NULL;

	while( m->bonds ) {
		Bond	*b;
		
		b = m->bonds;
		m->bonds = m->bonds->next;
		if( b->flag & flag ) {
			if( prev ) {
				prev->next = b->next;
			} else {
				m->bonds = b->next;
			}
			FREE( b );
		} else {
			prev = b;
		}
	}
}

/*------------------------------------------------------------
 *static void TestBonded( Molecule *m, Atom *src, Atom *dst );
 *------------------------------------------------------------*/

static void TestBonded( Molecule *m, Atom *src, Atom *dst )
{
	double	dist, max, len2;

	if( m->fast_bond_calc ) {
		if( src->hydrogen || dst->hydrogen ) {
			dist = MAX_HBOND_DIST;
		} else {
			dist = MAX_BOND_DIST;
		}
	} else {
		dist = (elements[GetElemNumber(src)].covalrad +
			elements[GetElemNumber(dst)].covalrad) / 250. + 0.5;
	}
	max = dist*dist;
	len2 = LENGTH2( src->x - dst->x, src->y - dst->y, src->z - dst->z );
	if( len2 < max && len2 > MIN_BOND_DIST * MIN_BOND_DIST ) {
		InsertBond( m, src, dst, BOND_CONECT );
		m->bonds_no++;
	}
	
}

/*------------------------------------------------------------
 *static void xyz2ijk( m, width, x, y, z, i, j, k );
 *------------------------------------------------------------*/

static void xyz2ijk( m, width, x, y, z, i, j, k )
	Molecule	*m;
	double		width[];
	double		x, y, z;
	int		*i, *j, *k;
{
	double	dx, dy, dz;

	dx = x - m->min[0];
	dy = y - m->min[1];
	dz = z - m->min[2];
	*i = (CUBE_SIZE-1) * dx / width[0];
	*j = (CUBE_SIZE-1) * dy / width[1];
	*k = (CUBE_SIZE-1) * dz / width[2];
	*i = MIN( MAX( 0, *i ), CUBE_SIZE-1 );
	*j = MIN( MAX( 0, *j ), CUBE_SIZE-1 );
	*k = MIN( MAX( 0, *k ), CUBE_SIZE-1 );
}

/*------------------------------------------------------------
 *static void CreateMoleculeBonds( Molecule *m );
 *------------------------------------------------------------*/

static void CreateMoleculeBonds( Molecule *m )
{
	Chain	*c;
	Group	*g;
	Atom	*a1, *a2;
	double	width[3];
	int	i, j, k;
	int	il, jl, kl, ih, jh, kh;

	FreeBonds( m, BOND_CONECT );
	m->bonds_no = 0;
	for( i = 0; i < CUBE_SIZE; i++ ) {
		for( j = 0; j < CUBE_SIZE; j++ ) {
			for( k = 0; k < CUBE_SIZE; k++ ) {
				m->cube[i][j][k] = NULL;
			}
		}
	}

	for( i = 0; i < 3; i++ )
		width[i] = m->max[i] - m->min[i];

	FOR_EACH_ATOM( c, g, a1 ) {
		xyz2ijk( m, width,
			a1->x - MAX_BOND_DIST,
			a1->y - MAX_BOND_DIST,
			a1->z - MAX_BOND_DIST,
			&il, &jl, &kl );
		xyz2ijk( m, width,
			a1->x + MAX_BOND_DIST,
			a1->y + MAX_BOND_DIST,
			a1->z + MAX_BOND_DIST,
			&ih, &jh, &kh );
		for( i = il; i <= ih; i++ ) {
			for( j = jl; j <= jh; j++ ) {
				for( k = kl; k <= kh; k++ ) {
					a2 = m->cube[i][j][k];
					while( a2 ) {
						TestBonded( m, a1, a2 );
						a2 = a2->roommate;
					}
				}
			}
		}
		
		xyz2ijk( m, width, a1->x, a1->y, a1->z, &i, &j, &k );
		a1->roommate = m->cube[i][j][k];
		m->cube[i][j][k] = a1;
	}
}

/*------------------------------------------------------------
 *static int GetElemNumber( aptr );
 *------------------------------------------------------------*/

static int GetElemNumber( aptr )
	Atom	*aptr;
{
	char	*ptr;
	char	ch;

	ptr = elem_desc[aptr->ref_no];
	ch = ptr[1];

	switch( *ptr )
	{   case(' '):  switch( ch )
	    {   case('B'):  return(  5 );
		case('C'):  return(  6 );
		case('D'):  return(  1 );
		case('F'):  return(  9 );
		case('H'):  return(  1 );
		case('I'):  return( 53 );
		case('K'):  return( 19 );
		case('L'):  return(  1 );
		case('N'):  return(  7 );
		case('O'):  return(  8 );
		case('P'):  return( 15 );
		case('S'):  return( 16 );
		case('U'):  return( 92 );
		case('V'):  return( 23 );
		case('W'):  return( 74 );
		case('Y'):  return( 39 );
	    }
		break;

	    case('A'):  switch( ch )
	    {   case('C'):  return( 89 );
		case('G'):  return( 47 );
		case('L'):  return( 13 );
		case('M'):  return( 95 );
		case('R'):  return( 18 );
		case('S'):  return( 33 );
		case('T'):  return( 85 );
		case('U'):  return( 79 );
	    }
		break;

	    case('B'):  switch( ch )
	    {   case('A'):  return( 56 );
		case('E'):  return(  4 );
		case('I'):  return( 83 );
		case('K'):  return( 97 );
		case('R'):  return( 35 );
	    }
		break;

	    case('C'):  switch( ch )
	    {   case('A'):  return( 20 );
		case('D'):  return( 48 );
		case('E'):  return( 58 );
		case('F'):  return( 98 );
		case('L'):  return( 17 );
		case('M'):  return( 96 );
		case('O'):  return( 27 );
		case('R'):  return( 24 );
		case('S'):  return( 55 );
		case('U'):  return( 29 );
	    }
		break;

	    case('D'):  if( ch=='Y' )
		    return( 66 );
		break;

	    case('E'):  if( ch=='R' )
	    {   return( 68 );
	    } else if( ch=='S' )
	    {   return( 99 );
	    } else if( ch=='U' )
		    return( 63 );
		break;

	    case('F'):  if( ch=='E' )
	    {   return(  26 );
	    } else if( ch=='M' )
	    {   return( 100 );
	    } else if( ch=='R' )
		    return(  87 );
		break;

	    case('G'):  if( ch=='A' )
	    {   return( 31 );
	    } else if( ch=='D' )
	    {   return( 64 );
	    } else if( ch=='E' )
		    return( 32 );
		break;

	    case('H'):  if( ch=='E' )
	    {   return(  2 );
	    } else if( ch=='F' )
	    {   return( 72 );
	    } else if( ch=='G' )
	    {   return( 80 );
	    } else if( ch=='O' )
		    return( 67 );
		break;

	    case('I'):  if( ch=='N' )
	    {   return( 49 );
	    } else if( ch=='R' )
		    return( 77 );
		break;

	    case('K'):  if( ch=='R' )
		    return( 36 );
		break;

	    case('L'):  if( ch=='A' )
	    {   return(  57 );
	    } else if( ch=='I' )
	    {   return(   3 );
	    } else if( (ch=='R') || (ch=='W') )
	    {   return( 103 );
	    } else if( ch=='U' )
		    return(  71 );
		break;

	    case('M'):  if( ch=='D' )
	    {   return( 101 );
	    } else if( ch=='G' )
	    {   return(  12 );
	    } else if( ch=='N' )
	    {   return(  25 );
	    } else if( ch=='O' )
		    return(  42 );
		break;

	    case('N'):  switch( ch )
	    {   case('A'):  return(  11 );
		case('B'):  return(  41 );
		case('D'):  return(  60 );
		case('E'):  return(  10 );
		case('I'):  return(  28 );
		case('O'):  return( 102 );
		case('P'):  return(  93 );
	    }
		break;

	    case('O'):  if( ch=='S' )
		    return( 76 );
		break;

	    case('P'):  switch( ch )
	    {   case('A'):  return( 91 );
		case('B'):  return( 82 );
		case('D'):  return( 46 );
		case('M'):  return( 61 );
		case('O'):  return( 84 );
		case('R'):  return( 59 );
		case('T'):  return( 78 );
		case('U'):  return( 94 );
	    }
		break;

	    case('R'):  switch( ch )
	    {   case('A'):  return( 88 );
		case('B'):  return( 37 );
		case('E'):  return( 75 );
		case('H'):  return( 45 );
		case('N'):  return( 86 );
		case('U'):  return( 44 );
	    }
		break;
		break;

	    case('S'):  switch( ch )
	    {   case('B'):  return( 51 );
		case('C'):  return( 21 );
		case('E'):  return( 34 );
		case('I'):  return( 14 );
		case('M'):  return( 62 );
		case('N'):  return( 50 );
		case('R'):  return( 38 );
	    }
		break;

	    case('T'):  switch( ch )
	    {   case('A'):  return( 73 );
		case('B'):  return( 65 );
		case('C'):  return( 43 );
		case('E'):  return( 52 );
		case('H'):  return( 90 );
		case('I'):  return( 22 );
		case('L'):  return( 81 );
		case('M'):  return( 69 );
	    }
		break;

	    case('X'):  if( ch=='E' )
		    return( 54 );
		break;

	    case('Y'):  if( ch=='B' )
		    return( 70 );
		break;

	    case('Z'):  if( ch=='N' )
	    {   return( 30 );
	    } else if( ch=='R' )
		    return( 40 );
		break;
	}

	if( (*ptr>='0') && (*ptr<='9') )
		if( (ch=='H') || (ch=='D') )
			return( 1 ); /* Hydrogen */

	return( 0 );
}

/*------------------------------------------------------------
 *		M A I N
 *------------------------------------------------------------*/

/* only for test
int main( int argc, char *argv[] )
{
	Molecule	*m;
	FILE		*src;

	if( argc != 2 ) exit( 1 );

	if( !(src = fopen( argv[1], "r" )) ) {
		fprintf( stderr, "can't open %s\n", argv[1] );
		exit( 1 );
	}

	m = MlclCreate( src );
#ifdef DEBUG
	fprintf( info_file, "atoms: %d, bonds: %d\n", 
		m->main_atoms_no, m->bonds_no );
#endif
	fclose( src );

	MlclFree( m );

	return 0;
}
*/
