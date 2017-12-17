
#ifndef	_GOD_HH_
#define	_GOD_HH_

#include	"land.hh"
#include	"body.hh"

class God {

friend	ostream &operator << ( ostream &s, const God &god );

//	public classes

    public:
	struct Motion {
		Motion() : wrist(0), angle(0) { }
		Motion( const String &wrist, double angle )
			: wrist(wrist), angle(angle) { }
		String	wrist;
		double	angle;
	};

	typedef	Array<Motion> Metamorphoses;
	typedef Map<String,double> WristVelocities;

//	ctors & dtor

    private:
	God( const God & );
	God &operator = ( const God & );

    public:
	God( const Body &body, const Point &speed, const Land &land );
		
//	opers

    public:

	const Body &body() const { return _body; }
	const Land &land() const { return _land; }
	const WristVelocities &wrist_velocities() const { return wv; }

	void time_elapsed( double dt, const Metamorphoses &m );

	
//	private data

    private:

	void	metamorphose( const Body &tbody, const Metamorphoses &m );
	void	balance( const Body &tbody );
	void	avoid_clash( const Body &tbody );

	void	move( const Body &tbody, double dt );

	void	rotate( const Body &tbody, double dt );
	double	get_w( const Body &tbody, double dt );
	double	get_w( double dt, const Point &centre, 
		      const Point &oldp, const Point &newp );
	int	get_axis();
	void	minimize_cog_motion( const Body &tbody );
	bool	avoid_clash_rotate( int axis );

	void	fix_axis_position();
	const Point axis_point();
	void	update_wv( const Body &tbody, double dt );

	Body	_body;
	Land	_land;
	int	axis; // index into the body.points() array or -1
	double	w; // in radian
	WristVelocities wv;

	Point	v;
	Point	real_v; // velocity before moving
	double	pickup; // absolute value of pickup
	double	J;
	double	M; // momentum
};


#endif // _GOD_HH_
