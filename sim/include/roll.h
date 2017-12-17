
#ifndef	ROLL_H
#define	ROLL_H

#include	<stdio.h>
#include	<tinyModel.h>
#include	"matrix.h"

typedef struct {
        double  specular_r, specular_g, specular_b;
        double  ambient_r, ambient_g, ambient_b;
        double  diffuse_r, diffuse_g, diffuse_b;
} Color;

typedef struct _Model	*Model;

extern Model mdlCreate();
extern void mdlFree( Model m );
extern int mdlSave( Model m, FILE * );

extern void mdlAddRoll( Model m, char *name,
		   double x1, double y1, double z1,
		   double x2, double y2, double z2, 
		   double r, Color *color1 );

#endif /* ROLL_H */
