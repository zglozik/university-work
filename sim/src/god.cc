
#include	"god.hh"
#include	"list.hh"
#include	"numanal.hh"

// ----------------------------------------------------------------------
//	God( const Body &body )
// ----------------------------------------------------------------------

God::God( const Body &body, const Point &speed, const Land &land ) 
: _body(body), _land(land), axis(-1), w(0), v(speed), real_v(speed), pickup(0),
  M(0)
{
	J = _body.J( axis_point() );
	for( Body::WristStore::iterator w = _body.wrists().begin();
	    w != _body.wrists().end(); w++ ) {
		wv[ _body.wrists().key(w) ] = 0;
	}
}

// ----------------------------------------------------------------------
//	time_elapsed( double dt, const Metamorphoses &m )
// ----------------------------------------------------------------------

void God::time_elapsed( double dt, const Metamorphoses &m )
{
	Body tbody = _body;

	metamorphose( tbody, m );
	move( tbody, dt );
	rotate( tbody, dt );
	fix_axis_position();
	update_wv( tbody, dt );

	Point ds = _body.centre_of_gravity() - tbody.centre_of_gravity();
	pickup = 2*abs(ds-v*dt) / (dt*dt);
	real_v = v = 2*ds/dt - v;
}

// ----------------------------------------------------------------------
//	metamorphose( const Body &tbody, const Metamorphoses &m )
// ----------------------------------------------------------------------

void God::metamorphose( const Body &tbody, const Metamorphoses &m )
{
	for( Metamorphoses::iterator i = m.begin(); i != m.end(); i++ ) {
		_body.bend( (*i).wrist, (*i).angle );
	}
	_body.move( tbody.centre_of_gravity() );
	balance( tbody );
	
	double L = J * w;
	J = _body.J( axis_point() );
	w = equal(J,0) ? 0 : L / J;
}

// ----------------------------------------------------------------------
//	balance( const Body &tbody )
// ----------------------------------------------------------------------

void God::balance( const Body &tbody )
{
	double	sum1 = 0;
	double	sum2 = 0;

	for( Body::PartStore::iterator part = _body.parts().begin(),
	    tpart = tbody.parts().begin();
	    part != _body.parts().end(); part++, tpart++ ) {
		Point d1 = (*tpart).centre_of_gravity() - 
			tbody.centre_of_gravity();
		Point d2 = (*part).centre_of_gravity() -
			_body.centre_of_gravity();

		if( Point(0) < d1 && Point(0) < d2 ) {
			double deg_diff = alpha( d1, d2 );

			if( normal(vector_mul(d1,d2)) == Point(0,-1,0) ) {
				deg_diff *= -1;
			}
			double product = square(abs((d1 + d2) / 2.)) * 
				(*part).weight();
			sum1 += product	* deg_diff;
			sum2 += product;
		}
	}
	if( !equal(sum2, 0) ) {
		double degree = -sum1 / sum2;
		_body.rotate( degree, _body.centre_of_gravity() );
	}
}

// ----------------------------------------------------------------------
//	move( const Body &tbody, double dt )
// ----------------------------------------------------------------------

void God::move( const Body &tbody, double dt )
{
	bool clash = false;
	for( Body::PointStore::iterator point = _body.points().begin();
	    point != _body.points().end() && !clash; point++ ) {
		clash = v != Point(0) &&
			(_land.is_surface(*point) || _land.inside(*point)) &&
			alpha(v, _land.normal(*point + _land.depth(*point)))
			>= 90;
	}
	if( !clash ) {
		Point g( 0, 0, -10 );
		Point rel = g * dt * dt / 2.0 + v * dt;
		_body.move_rel( rel );
	}
	avoid_clash( tbody );
}

// ----------------------------------------------------------------------
//	rotate( const Body &tbody, double dt )
// ----------------------------------------------------------------------

void God::rotate( const Body &tbody, double dt )
{
	int old_axis = axis;

	axis = get_axis();
	if( old_axis != axis ) {
		w = get_w( tbody, dt );
	}

	double a;
	M = 0;
	Point g(0, 0, -10);
	for( Body::PartStore::iterator part = _body.parts().begin();
	    part != _body.parts().end(); part++ ) {
		Point r = (*part).centre_of_gravity() - axis_point();
		Point k = vector_mul( r, (*part).weight() * g);
		double M0 = abs(k);
		if( k != Point(0) && alpha(Point(0,-1,0),k) < 90 ) {
			M0 *= -1;
		}
		M += M0;
	}
	a = equal(J,0) ? 0 : M / J;

	Point old_cog = _body.centre_of_gravity();
	double diff = a*dt*dt/2 + w * dt;
	_body.rotate( rad2deg(diff), axis < 0 ?
		     _body.centre_of_gravity() : _body.points()[axis] );

	if( avoid_clash_rotate( axis ) ) {
		w = 0;
	} else {
		w += a * dt;
	}
}

// ----------------------------------------------------------------------
//	get_w( const Body &tbody, double dt )
// ----------------------------------------------------------------------

double God::get_w( const Body &tbody, double dt )
{
	double w;
	
	if( axis >= 0 ) {
		Point	g( 0, 0, -10 );
		w = get_w( dt, _body.points()[axis], tbody.centre_of_gravity(),
			  tbody.centre_of_gravity() + v*dt + g*dt*dt/2.0 );
	} else {
		Point centre = _body.centre_of_gravity();
		double L = 0;
		for( Body::PartStore::iterator part = _body.parts().begin(),
		    tpart = tbody.parts().begin();
		    part != _body.parts().end(); part++, tpart++ ) {
			double r;
			r = abs((*part).centre_of_gravity() - centre);
			L += square(r) * (*part).weight() * 
				get_w( dt, centre,
				      (*tpart).centre_of_gravity(),
				      (*part).centre_of_gravity() );
		}
		w = equal(J,0) ? 0 : L / J;
	}
	return w;
}

// ----------------------------------------------------------------------
//	get_w( double dt, const Point &centre, 
//		const Point &oldp, const Point &newp ) 
// ----------------------------------------------------------------------

double God::get_w( double dt, const Point &centre, 
		  const Point &oldp, const Point &newp ) 
{
	double w;
	Point v = (newp - oldp) / dt;
	Point r = oldp - centre;
	
	if( r != Point(0) ) {
		w = normal(vector_mul(r, Point(0,-1,0))) * v / abs(r);
	} else {
		w = 0;
	}
	return w;
}

// ----------------------------------------------------------------------
//	get_axis()
// ----------------------------------------------------------------------

int God::get_axis()
{
	const Body::PointStore &pts = _body.points();
	Point		g( 0, 0, -10 );
	double		m = _body.weight();
	Point		centre = _body.centre_of_gravity();
	List<int>	list;

	for( int k = 0; k < pts.size(); k++ ) {
		if( _land.is_surface(pts[k]) && 
		   alpha(g, _land.normal(pts[k])) > 90 ) {
			list.push_back( k );
		}
	}
	bool found = false;
	List<int>::iterator i;
	for( i = list.begin(); i != list.end() && !found; i++ ) {

		Point M = vector_mul( m*g, centre - pts[*i] );
		found = true;
		for( List<int>::iterator j = list.begin(); 
		    j != list.end() && found; j++ ) {
			Point M1 = vector_mul( _land.normal(pts[*j]),
					      pts[*j] - pts[*i] );
			found = M == Point(0) || M1 == Point(0) ||
				alpha( M, M1 ) < 90;
		}
	}

	return found ? *--i : -1;
}

// ----------------------------------------------------------------------
//	avoid_clash( const Body &tbody )
// ----------------------------------------------------------------------

void God::avoid_clash( const Body &tbody )
{
	for( int i = 0; i < _body.points().size(); i++ ) {
		if( _land.inside(_body.points()[i]) ||
		   _land.is_surface(_body.points()[i]) ) {
			if( _land.is_surface( tbody.points()[i] ) ) {
				_body.move_rel( tbody.points()[i] -
					       _body.points()[i] );
			} else if( _land.inside(_body.points()[i]) ) {
				Point depth = _land.depth( _body.points()[i] );
				_body.move_rel( depth );
			}
		}
	}
	minimize_cog_motion( tbody );
}


// ----------------------------------------------------------------------
//	minimize_cog_motion( const Body &tbody ) 
// ----------------------------------------------------------------------

void God::minimize_cog_motion( const Body &tbody ) 
{
	const Body::PointStore &pts = _body.points();
	const Body::PointStore &tpts = tbody.points();
	List<int> list;

	Point oc = tbody.centre_of_gravity();
	Point nc = _body.centre_of_gravity();
	for( int k = 0; k < pts.size(); k++ ) {
		if( _land.is_surface(pts[k]) && tpts[k]!=oc && pts[k]!=nc ) {
			list.push_back( k );
		}
	}

	bool good = false;
	double beta; // says how much we should rotate the body
	List<int>::iterator p1;
	for( p1 = list.begin(); p1 != list.end() && !good; p1++ ) {

		beta = alpha( oc-tpts[*p1], nc-pts[*p1] );
		if( !equal(beta,0) && !equal(beta,180) &&
		   alpha( vector_mul(oc-tpts[*p1], nc-pts[*p1]), 
			 Point(0,1,0) ) < 90 ) {
			beta *= -1;
		}
		Transform<double> rotate;
		rotate.rotate( beta, 'y' );

		good = true;
		for( List<int>::iterator p2 = list.begin(); 
		    p2 != list.end() && good; p2++ ) {
			Point np2 = rotate.transform( pts[*p2] - pts[*p1] );
			Point diff = np2 - (pts[*p2] - pts[*p1]);
			good = diff == Point(0) ||
				alpha( _land.normal(pts[*p2]), diff ) < 90;
		}
	}
	if( good ) {
		--p1;
		_body.rotate( beta, pts[*p1] );
		avoid_clash_rotate( *p1 );
	}
}

// ----------------------------------------------------------------------
//	avoid_clash_rotate()
// ----------------------------------------------------------------------

bool God::avoid_clash_rotate( int axis )
{
	Point axis_point = axis >= 0 ?
		_body.points()[axis] : _body.centre_of_gravity();
	bool found = false;
	for( int i = 0; i < _body.points().size(); i++ ) {
		Point r = _body.points()[i] - axis_point;
		if( i != axis && r != Point(0) && 
		   _land.inside(_body.points()[i]) ) {
			
			Point d = _land.depth( _body.points()[i] );
			double deg = find_intersection( axis_point,
						       _body.points()[i],
						       normal(d),
						       _body.points()[i] + d );

			_body.rotate( deg, axis_point );
			found = true;
		}
	}
	return found;
}

// ----------------------------------------------------------------------
//	fix_axis_position()
// ----------------------------------------------------------------------

void God::fix_axis_position()
{
	if( axis >= 0 ) {
		_body.move_rel( _land.depth( axis_point() ) );
	}
}

// ----------------------------------------------------------------------
//	update_wc( const Body &tbody, double dt )
// ----------------------------------------------------------------------

void God::update_wv( const Body &tbody, double dt )
{
	for( WristVelocities::iterator w = wv.begin();
	    w != wv.end(); w++ ) {
		wv[ wv.key(w) ] = (_body.alpha(wv.key(w)) - 
			tbody.alpha(wv.key(w))) / dt;
	}
}

// ----------------------------------------------------------------------
//	axis_point()
// ----------------------------------------------------------------------

const Point God::axis_point()
{
	return axis >= 0 ? _body.points()[axis] : _body.centre_of_gravity();
}

// ----------------------------------------------------------------------
//	ostream &operator << ( ostream &s, const God &god )
// ----------------------------------------------------------------------

ostream &operator << ( ostream &s, const God &god )
{
	s << god._body.centre_of_gravity() << '\t';
	s << god.real_v << '\t';
	s << god.pickup << '\t';
	s << god.M << '\t'; // momentum
	copy( god.wv.begin(), god.wv.end(), ostream_iterator<double>(s, " ") );
	s << god._body;
	return s;
}

// ----------------------------------------------------------------------
/*
#include	<unistd.h>
#include	"hermite.hh"

int main()
{
	typedef	Hermite<double>	Spline;
	Body::PointStore	*points = new Body::PointStore;
	Body::PartStore		*parts = new Body::PartStore;
	Body::WristStore	*wrists = new Body::WristStore;
	Viewer	viewer;

	points->push_back( Point(0,0,0) );
	points->push_back( Point(0.5,0,0) );
	points->push_back( Point(0,0,0.5) );
	points->push_back( Point(0,0,1.4) );
	points->push_back( Point(0.5,0,0) );

	parts->push_back( Body::Part(&(*points)[0], &(*points)[1], 1/2., 5) );
	parts->push_back( Body::Part(&(*points)[1], &(*points)[2], 2/3., 15) );
	parts->push_back( Body::Part(&(*points)[2], &(*points)[3], 4/4., 45) );
//	parts->push_back( Body::Part(&(*points)[0], &(*points)[4], 1/2., 5) );

	(*wrists)["knee"] = 
		Body::Wrist( (*parts)[0], (*parts)[1], Point(0,1,0) );
	(*wrists)["waist"] = 
		Body::Wrist( (*parts)[1], (*parts)[2], Point(0,-1,0) );
//	(*wrists)["ankle"] = 
//		Body::Wrist( (*parts)[0], (*parts)[3], Point(0,1,0) );

	Array<Spline>		splines;
	Spline::PointStore	positions;
	positions.resize(4);
	positions[0] = Spline::Parameter( 0, 45 );
	positions[1] = Spline::Parameter( 20, 140 );
	positions[2] = Spline::Parameter( 30, 70 );
	positions[3] = Spline::Parameter( 100, 50 );
	splines.push_back( Spline( positions, 95/15., 0 ) );
	positions.resize(4);
	positions[0] = Spline::Parameter( 0, 45 );
	positions[1] = Spline::Parameter( 15, 80 );
	positions[2] = Spline::Parameter( 30, 50 );
	positions[3] = Spline::Parameter( 100, 45 );
	splines.push_back( Spline( positions, 35/15., 0 ) );
	positions.resize(3);
	positions[0] = Spline::Parameter( 0, 0 );
	positions[1] = Spline::Parameter( 80, 0 );
	positions[2] = Spline::Parameter( 100, 200 );
	splines.push_back( Spline( positions, 1, 0 ) );

	Body	body( points, parts, wrists, viewer );
	body.bend( "knee", splines[0](0) );
	body.bend( "waist", splines[1](0) );
	body.move_rel( -body.points()[0] );
	body.rotate( -45, 0 );

	God	god( body, Land(viewer) );

	sleep( 5 );
	for( int i = 0; i <= 100; i++ ) {
		God::Metamorphoses motions(2);
		motions[0] = God::Motion( "knee", splines[0](i) );
		motions[1] = God::Motion( "waist", splines[1](i) );
//		motions[2] = God::Motion( "ankle", splines[2](i) );

//		cout << splines[0](i) << ',' << splines[1](i) << endl;
		viewer.begin();
		god.land().draw();
		god.body().draw();
		viewer.end();
		god.time_elapsed( 0.01, motions );
		usleep( 10 );
	}
	cout << "done\n";
	God::Metamorphoses motions(0);
	for( int i = 0; i <= 1000; i++ ) {
		viewer.begin();
		god.land().draw();
		god.body().draw();
		viewer.end();
		god.time_elapsed( 0.01, motions );
		usleep( 10 );
	}

	fgetc( stdin );

	return 0;
}
*/
