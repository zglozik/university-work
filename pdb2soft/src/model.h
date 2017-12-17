
#ifndef	MODEL_H
#define	MODEL_H

#include	"molecule.h"

typedef struct _Model	*Model;

extern Model	mdlCreate( int horizontal, int vertical, 
			  int lo_step, int la_step, int la_base );
extern void	mdlSave( Model m, FILE *file );
extern void	mdlFree( Model m );

extern void	mdlAddSphere( Model m, char *name, 
			     double x, double y, double z,
			     double r, Color *color );
extern void	mdlAddRoll( Model m, char *name,
			   double x1, double y1, double z1,
			   double x2, double y2, double z2, 
			   double r, Color *color1, Color *color2 );

#endif /* MODEL_H */
