
#ifndef	MOLECULE_H
#define	MOLECULE_H

#include	<stdio.h>
#include	"stdtypes.h"

#define	BOND_CONECT	0x01
#define	BOND_IMPLICIT	0x02

typedef struct _Atom {
	double		x, y, z;/* coordinates in the PDB file */
	double		r;	/* radius */
	int		serno;	/* atom serial number */
	int		ins_code; /* insertion code */
	int		ref_no;	/* index in ElemDesc array */
	bool		hydrogen; /* is this atom hydrogen? */
	struct _Atom	*next;
	struct _Atom	*roommate; /* roommate in the cube */
} Atom;

typedef struct _Bond {
	Atom		*src;	/* source atom */
	Atom		*dst;	/* destination atom */
	int		flag;	/* BOND_CONECT or BOND_IMPLICIT */
	struct _Bond	*next;
} Bond;

typedef struct _Group {
	Atom		*atoms;	/* all of the atoms in the molecule */
	int		seqno;	/* group sequence number */
	int		resno;	/* index in residue array */
	struct _Group	*next;
} Group;

typedef struct _Chain {
	Group		*groups;/* all of the groups in the chain */
	char		ident;	/* chain identifier */
	struct _Chain	*next;
} Chain;


#define	CUBE_SIZE	20
	
typedef struct _Molecule {
	Bond	*bonds;		/* all of the bonds in the molecule */
	Chain	*chains;	/* all of the chains in the molecule */

/* private */
	Atom	*last_atom;	/* last atom in PDB file to connect to*/
	int	main_atoms_no;	/* number of main atoms in molecule */
	int	heta_atoms_no;	/* number of heterogen atoms in molecule */
	int	bonds_no;	/* number of bonds in molecule */
	bool	first_atom;	/* is there any atom in the structure? */
	bool	change_chain;	/* do we have to change chain next time? */
	bool	fast_bond_calc;	/* how to test bonds between atoms */
	double	max[3], min[3];	/* maximum and minimum coordinates */
	Atom	*cube[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE];
				/* to refer to atoms which are in the cube */
} Molecule;

/* information about elements */

typedef struct {
	char symbol[2];
	int covalrad;
	int cpkcol;
	double radius;
	char *name;
} ElemStruct;

#define MAXELEM_NO	104

extern ElemStruct element[MAXELEM_NO];

typedef struct {
	double	specular_r, specular_g, specular_b;
	double	ambient_r, ambient_g, ambient_b;
	double	diffuse_r, diffuse_g, diffuse_b;
} Color;

#define	MAXCOLOR_NO	22

extern Color colors[MAXCOLOR_NO];

extern Molecule	*MlclCreate( FILE *file );
extern void	MlclFree( Molecule *m );

extern Color	*GetColor( Atom *a );
extern char	*GetName( Atom *a );
extern double	GetRadius( Atom *a );

#endif /* MOLECULE_H */
