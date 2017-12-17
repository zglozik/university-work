
#ifndef	_WRIST_MOTION_HH_
#define	_WRIST_MOTION_HH_

#include	"god.hh"
#include	"expression.hh"
#include	"hermite.hh"

class Segment;

class WristMotion {

	WristMotion( const WristMotion & );
	WristMotion &operator = ( const WristMotion & ); 

//	dtor

    public:
	WristMotion();
	virtual	~WristMotion();

//	opers

    public:
	void	start( double t0 );
	bool	add_point( double t, Expression *e );
	bool	add_range( double t0, double t1, Expression *e = 0 );
	bool	end();
	
	bool	evaluate( double t, double &result );
	double	last_t();

	SymbolTable &symbol_table() { return st; }

//	private data

    private:
	bool	make_constant( double t0, double t1 );
	bool	make_spline( double t0, double t1 );
	bool	make_expression( double t0, double t1, Expression *e );

	void	destroy_segments();

	typedef	List<Segment *>			SegmentStore;
	typedef	List< Pair<double,double> >	PointStore;

	SymbolTable	st;
	SegmentStore	segments;

	PointStore	tmp_points;
	double		t0;
	bool		done;
};

#endif // _WRIST_MOTION_HH_
