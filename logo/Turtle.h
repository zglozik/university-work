
#ifndef	TURTLE_H
#define	TURTLE_H

#include	"gc.h"

#define	String	XString
#include	<Xm/Xm.h>
#undef		String

class Turtle : virtual public Reference {

    public:

	Turtle( int &argc, char *argv[] );
virtual	~Turtle();

	void	main_loop() 
	{
		XtVaSetValues( _w, XmNbackgroundPixmap, _pixmap,
			      NULL );
		XtAppMainLoop( _app );
	}

	void	forward( double );
	void	backward( double );
	void	left( double );
	void	right( double );
	void	pen_up();
	void	pen_down();
	void	direction( double );
	void	position( double, double );

    private:
	bool	_draw;
	double	_direction;
	int	_x, _y;

	XtAppContext	_app;
	Widget		_w;
	Pixmap		_pixmap;
	GC		_gc;
};

#endif /* TURTLE_LH */
