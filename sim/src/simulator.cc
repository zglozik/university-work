
#include	"simulator.hh"
#include	<unistd.h>
#include	<time.h>
#include	<fstream.h>
#include	<string.h>

#define	TIME_SLICE	0.001
#define	SKIP_FRAMES	0

// ----------------------------------------------------------------------
//	Simulator( int argc, char *argv[] );
// ----------------------------------------------------------------------

Simulator::Simulator( int argc, char *argv[] )
{
	extern char	*optarg;
	extern int	optind;

	delay = 1;
	start_frame = 0;
	end_frame = 10000;
	output_file[0] = '\0';

	bool error = false;
	int c = 0;
	while( c != EOF ) {
		switch( c = getopt( argc, argv, "d:s:e:o:" ) ) {
		    case 'd':
			delay = atoi(optarg);
			break;
		    case 's':
			start_frame = atoi(optarg);
			break;
		    case 'e':
			end_frame = atoi(optarg);
			break;
		    case 'o':
			strcpy( output_file, optarg );
			break;
		    case ':':
			cerr << "missing parameter\n";
			error = true;
			break;
		    case '?':
			error = true;
			break;
		}
	}
	if( !error ) {
		if( optind == argc ) {
			input_file[0] = '\0';
		} else if( optind == argc-1 ) {
			strcpy( input_file, argv[optind] );
		} else {
			error = true;
		}
	}
	if( start_frame > end_frame ) {
		cerr << "start_frame must be greater or ";
		cerr << "equal than end_frame\n";
		error = true;
	}
	if( error ) {
		cerr << "usage: sim [-d delay_factor] [-s <start_frame>] ";
		cerr << "[-e <end_frame>] [-o <file_name>] ";
		cerr << "[<file_name>]\n";
		exit(1);
	}
}

// ----------------------------------------------------------------------
//	run();
// ----------------------------------------------------------------------

void Simulator::run()
{
	FILE	*input;
	FILE	*output = NULL;
	Point	speed;

	if( input_file[0] == '\0' || !strcmp(input_file,"-") ) {
		input = stdin;
	} else if( (input = fopen(input_file,"r")) == NULL ) {
		cerr << "I can't open file: " << input_file << endl;
		exit( 1 );
	}
	if( !strcmp(output_file, "-") ) {
		output = stdout;
	} else if( output_file[0] != '\0' && 
		  (output = fopen(output_file,"w")) == NULL ) {
		cerr << "I can't open file: " << output_file << endl;
		exit( 1 );
	}
			
	Viewer	viewer;
	Body 	body( viewer );
	Map<String, WristMotion *> wrist_motions;

	bool ok = parse_input( body, wrist_motions, speed, viewer, input );
	fclose( input );
	if( ok ) {
		God	*god;
		while( ok ) {
			god = new God( body, speed, Land(viewer) );
			::sleep( 1 );
			ok = play( 0, *god, wrist_motions, viewer, output );
			delete god;
			if( output ) {
				fclose( output );
				output = NULL;
			}
		}
	}
	if( output ) fclose( output );
	while( !wrist_motions.is_empty() ) {
		delete *wrist_motions.begin();
		wrist_motions.destroy( wrist_motions.begin() );
	}

}

// ----------------------------------------------------------------------
//	init_play( God &god, Viewer &viewer )
// ----------------------------------------------------------------------

void Simulator::init_play( God &god, Viewer &viewer )
{
	viewer.init_frames( start_frame );
	
	routes.resize( god.body().parts().size() + 1 );
	for( int i = 0; i < routes.size(); i++ ) {
		empty( routes[i] );
	}
	update_routes( god.body() );
}

// ----------------------------------------------------------------------
//	play( double t0, God &god, Map<String, WristMotion *> &wms,
//	     Viewer &viewer, FILE *output );
// ----------------------------------------------------------------------

bool Simulator::play( double t0, God &god, Map<String, WristMotion *> &wms,
		     Viewer &viewer, FILE *output )
{
	God::Metamorphoses motions;

	init_play( god, viewer );
	double	t1 = last_t( wms );
	int	frames = 0;
	int	skip_frames = 0;
	Body	old_body = god.body();
	bool	ok = read_metamorphoses( t0+TIME_SLICE, wms, motions );
#ifdef _SI_VIEWER
	viewer.begin();	// to draw icon
	god.body().draw();
	god.land().draw();
	viewer.end();
#endif
	for( double t = t0; 
	    (t <= t1 || god.body() != old_body) && ok && frames <= end_frame; 
	    t += TIME_SLICE ) {
		if( skip_frames-- == 0 ) {
			old_body = god.body();
			skip_frames = SKIP_FRAMES;
			if( frames >= start_frame ) {
				viewer.begin();
				god.body().draw();
				god.land().draw();
				draw_routes( viewer );
				viewer.end();
				sleep();
			}
			write_state( god, t0, t, output );
			frames++;
		}
		god.time_elapsed( TIME_SLICE, motions );
		update_routes( god.body() );

		if( abs(god.body().centre_of_gravity()) > 10 ) {
			cerr << "body went too far" << endl;
			ok = false;
		}
		ok = ok && read_metamorphoses( t+2*TIME_SLICE, wms, motions );
	}

	return ok;
}

// ----------------------------------------------------------------------
//	last_t( const Map<String, WristMotion *> &wms )
// ----------------------------------------------------------------------

double Simulator::last_t( const Map<String, WristMotion *> &wms )
{
	TEST_EXPR( !wms.is_empty() );

	Map<String, WristMotion *>::iterator i = wms.begin();
	double	t = (*i)->last_t();
	for( i++; i != wms.end(); i++ ) {
		t = max( t, (*i)->last_t() );
	}
	return t;
}

// ----------------------------------------------------------------------
//	update_routes( const Body &body )
// ----------------------------------------------------------------------

void Simulator::update_routes( const Body &body )
{
	TEST_EXPR( routes.size() == body.parts().size()+1 );

	if( body.show_centre() ) {
		add_to_route( 0, body.centre_of_gravity() );
	}
	int i = 1;
	for( Body::PartStore::iterator part = body.parts().begin();
	    part != body.parts().end(); part++ ) {
		if( (*part).show_centre() ) {
			add_to_route( i, (*part).centre_of_gravity() );
		}
		i++;
	}
}

// ----------------------------------------------------------------------
//	add_to_route( int i, const Point &point )
// ----------------------------------------------------------------------

void Simulator::add_to_route( int i, const Point &point )
{
	if( routes[i].is_empty() || abs(routes[i].back() - point) > 0.2 ) {
		routes[i].push_back( point );
	}
}

// ----------------------------------------------------------------------
//	draw_routes( Viewer &viewer )
// ----------------------------------------------------------------------

void Simulator::draw_routes( Viewer &viewer )
{
	viewer.set_width( 0.05 );
	for( int i = 0; i < routes.size(); i++ ) {
		if( !routes[i].is_empty() ) {
			viewer.move_to( routes[i].front() );
			List<Point>::iterator p = routes[i].begin(); 
			for( p++; p != routes[i].end(); p++ ) {
				viewer.line_to( *p );
			}
		}
	}
}

// ----------------------------------------------------------------------
//	write_state( const God &god, double t0, double t, FILE *output )
// ----------------------------------------------------------------------

void Simulator::write_state( const God &god, double t0, double t, 
			    FILE *output )
{
	if( !output ) return;

	ofstream out;
	out.attach( fileno(output) );
	if( t == t0 ) {
		out << "time ";
		out << "centre_of_gravity(x) ";
		out << "centre_of_gravity(y) ";
		out << "centre_of_gravity(z) ";
		out << "centre_of_gravity(velocity(x)) ";
		out << "centre_of_gravity(velocity(y)) ";
		out << "centre_of_gravity(velocity(z)) ";
		out << "centre_of_gravity(abs(pickup)) ";
		out << "momentum ";
		God::WristVelocities::iterator w1;
		for( w1 = god.wrist_velocities().begin();
		    w1 != god.wrist_velocities().end(); w1++ ) {
			out << god.wrist_velocities().key(w1) << "(velocity) ";
		}
		for( Body::PartStore::iterator p = god.body().parts().begin();
		    p != god.body().parts().end(); p++ ) {
			out << god.body().parts().key(p) << "(angle) ";
			out << god.body().parts().key(p) << "(x) ";
			out << god.body().parts().key(p) << "(y) ";
			out << god.body().parts().key(p) << "(z) ";
		}
		Body::WristStore::iterator w2;
		for( w2 = god.body().wrists().begin();
		    w2 != god.body().wrists().end(); w2++ ) {
			out << god.body().wrists().key(w2) << "(angle) ";
			out << god.body().wrists().key(w2) << "(x) ";
			out << god.body().wrists().key(w2) << "(y) ";
			out << god.body().wrists().key(w2) << "(z) ";
		}
		out << endl;
	}
	out << t << '\t' << god << endl;
}

// ----------------------------------------------------------------------
//	read_metamorphoses( double t, Map<String, WristMotion *> &wms,
//			   God::Metamorphoses &motions );
// ----------------------------------------------------------------------

bool Simulator::read_metamorphoses( double t,
				   Map<String, WristMotion *> &wms,
				   God::Metamorphoses &motions )
{
	motions.resize( wms.size() );
	bool ok = true;
	int i = 0;
	for( Map<String, WristMotion *>::iterator w = wms.begin(); 
	    w != wms.end() && ok; w++ ) {
		double value;
		ok = (*w)->evaluate( t, value );
		if( !ok ) {
			cerr << "I can't evaluate expression for " <<
				wms.key(w) << ", at: " << t << endl;
		} else {
			motions[i++] = God::Motion( wms.key(w), value );
		}
	}

	return ok;
}

// ----------------------------------------------------------------------
//	sleep()
// ----------------------------------------------------------------------

void Simulator::sleep()
{
#ifdef _X_VIEWER
	struct timespec	tspec;
	tspec.tv_sec = 0;
	tspec.tv_nsec = 19999999L * delay; // 0.02 seconds
	nanosleep( &tspec, NULL );
#endif
}

// ----------------------------------------------------------------------

int main( int argc, char *argv[] )
{
	Simulator	simulator( argc, argv );

	simulator.run();

	return 0;
}
