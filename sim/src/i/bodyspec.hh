
#ifndef	_BODYSPEC_HH_
#define	_BODYSPEC_HH_

#include	"expression.hh"
#include	"bodybuilder.hh"
#include	"wristmotion.hh"

#define	MAX_ID_LEN	30

struct Boundaries {
	int	size;
	double	bounds[2];
};

struct PartPoint {
	char			id[MAX_ID_LEN+1];
	BodyBuilder::Endpoint	endpoint;
};

struct _Point {
	double	x, y, z;
};

extern int	yyline_no;

extern bool parse_input( Body &body, Map<String,WristMotion *> &wrist_motions,
			Point &s0, Viewer &viewer, FILE *input );

#endif // _BODYSPEC_HH_
