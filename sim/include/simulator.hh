
#ifndef	_SIMULATOR_HH_
#define	_SIMULATOR_HH_

#include	"bodyspec.hh"

class Simulator {

    private:
	Simulator( const Simulator & );
	Simulator &operator = ( const Simulator & );

//	ctors & dtor

    public:
	Simulator( int argc, char *argv[] );

//	opers

    public:
	void	run();

//	private data

    private:
	void	init_play( God &god, Viewer &viewer );
	bool	play( double t0, God &god, Map<String, WristMotion *> &wms,
		     Viewer &viewer, FILE *output );
	double	last_t( const Map<String, WristMotion *> &wms );

	void	update_routes( const Body &body );
	void	add_to_route( int i, const Point &point );
	void	draw_routes( Viewer &viewer );

	void	write_state( const God &god, double t0, double t, 
			    FILE *output );

	bool 	read_metamorphoses( double t, Map<String, WristMotion *> &wms,
				   God::Metamorphoses &motions );
	void	sleep();

	typedef	Array< List<Point> > Routes;

	Routes	routes;
	char	input_file[200];
	char	output_file[200];
	int	delay;
	int	start_frame;
	int	end_frame;
};

#endif // _SIMULATOR_HH_
