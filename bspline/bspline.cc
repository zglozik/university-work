
#include	"bspline.hh"
// #include	<funcs.hh>

//
//	Base class
//

//----------------------------------------------------------------------
//	Base::Base( uint n, const KnotVector &kv )
//----------------------------------------------------------------------

Base::Base( uint n, const KnotVector &kv ) : kv(kv), n(n)
{
	TEST_EXPR( (uint) kv.size() >= 2*n+2 );
	update_m( kv );
}

//----------------------------------------------------------------------
//	Base::Base( uint n, uint k, double t0, double t1 )
//----------------------------------------------------------------------

Base::Base( uint n, uint k, double t0, double t1 ) : kv(k), n(n)
{
	TEST_EXPR( k >= 2*n+2 );
	TEST_EXPR( t1 > t0 );

	for( uint i = 0; i < k; i++ )
		kv[i] = t0 + (t1-t0) * i / (k-1);
	update_m( kv );
}

//----------------------------------------------------------------------
//	double Base::operator () ( uint i, double t ) const
//----------------------------------------------------------------------

double Base::operator () ( uint i, double t ) const
{
	TEST_EXPR( t >= kv.front() && t <= kv.back() );
	TEST_EXPR( i <= kv.size() - 1 - (n+1) );

	return compute( i, n, t );
}

//----------------------------------------------------------------------
//	double Base::derivate( uint i, double t, uint k, bool &error ) const
//----------------------------------------------------------------------

double Base::derivate( uint i, double t, uint k, bool &error ) const
{
	TEST_EXPR( t >= kv.front() && t <= kv.back() );
	TEST_EXPR( i <= kv.size() - 1 - (n+1) );

	double rv;
	Multis::iterator multi = m.find( t );
	error = false;

	if( multi != m.end() && k != 0 && k + *multi > n ) {
		error = true;
		rv = 0;
	} else if( k <= n ) {
		rv = compute_derivative( i, n, t, k );
	} else {
		rv = 0;
	}

	return rv;
}

//----------------------------------------------------------------------
//	double Base::min_t() const
//----------------------------------------------------------------------

double Base::min_t() const
{
	return kv[n];
}

//----------------------------------------------------------------------
//	double Base::max_t() const
//----------------------------------------------------------------------

double Base::max_t() const
{
	return kv[kv.size()-1-n];
}

//----------------------------------------------------------------------
//	double Base::compute( uint i, uint n, double t )
//----------------------------------------------------------------------

double Base::compute( uint i, uint n, double t ) const
{
	double	value;

	if( t < kv[i] || t > kv[i+n+1] ) {
		value = 0;
	} else if( n == 0 ) {
		if( t >= kv[i] &&  (t < kv[i+1] || 
		   (equal(t,kv[i+1]) && equal(t,kv.back()))) ) {
			value = 1;
		} else {
			value = 0;
		}
	} else {
		value = 0;
		if( !equal( kv[i+n], kv[i] ) ) {
			value += (t-kv[i]) / (kv[i+n]-kv[i]) * 
				compute(i,n-1,t);
		}
		if( !equal( kv[i+n+1], kv[i+1] ) ) {
			value += (kv[i+n+1]-t) / (kv[i+n+1]-kv[i+1]) *
				compute(i+1,n-1,t);
		}
	}

	return value;
}

//----------------------------------------------------------------------
//	double Base::compute_derivative( uint i, uint n, 
//					double t, uint k ) const
//----------------------------------------------------------------------

double Base::compute_derivative( uint i, uint n, double t, uint k ) const
{
	double rv;

	if( t < kv[i] || t > kv[i+n+1] ) {
		rv = 0;
	} else if( k == 0 ) {
		rv = compute( i, n, t );
	} else {
		rv = 0;
		if( !equal( kv[i+n], kv[i] ) ) {
			rv = compute_derivative( i, n-1, t, k-1 ) /
				(kv[i+n] - kv[i]);
		}
		if( !equal( kv[i+n+1], kv[i+1] ) ) {
			rv -= compute_derivative( i+1, n-1, t, k-1 ) /
				(kv[i+n+1] - kv[i+1]);
		}
		rv *= n;
	}

	return rv;
}

//----------------------------------------------------------------------
//	void Base::update_m( const KnotVector &kv )
//----------------------------------------------------------------------

void Base::update_m( const KnotVector &kv )
{
	empty( m );

	for( KnotVector::iterator knot = kv.begin(); knot != kv.end(); ) {
		uint	multi = 1;
		double	value = *knot++;
		
		while( knot != kv.end() && equal(value, *knot) ) {
			multi++;
			knot++;
		}
		m[ value ] = multi;
	}
}

//----------------------------------------------------------------------
//	uint Base::K() const
//----------------------------------------------------------------------

uint Base::K() const
{
	return kv.size()-1-n;
}

//
//	BSplineCurve class
//

//----------------------------------------------------------------------
//	BSplineCurve::BSplineCurve( const BSplineCurve &b )
//----------------------------------------------------------------------

BSplineCurve::BSplineCurve( const BSplineCurve &b ) : base(0), points(0)
{
	if( b.base ) base = new Base( *b.base );
	if( b.points ) points = new Points( *b.points );
}

//----------------------------------------------------------------------
//	BSplineCurve::~BSplineCurve()
//----------------------------------------------------------------------

BSplineCurve::~BSplineCurve()
{
	delete base;
	delete points;
}

//----------------------------------------------------------------------
//	void BSplineCurve::set_param( uint n, const KnotVector &kv )
//----------------------------------------------------------------------

void BSplineCurve::set_param( uint n, const KnotVector &kv )
{
	delete base;
	base = new Base( n, kv );
}

//----------------------------------------------------------------------
//	void BSplineCurve::set_param( uint n, uint k, double t0, double t1 )
//----------------------------------------------------------------------

void BSplineCurve::set_param( uint n, uint k, double t0, double t1 )
{
	delete base;
	base = new Base( n, k, t0, t1 );
}

//----------------------------------------------------------------------
//	void BSplineCurve::set_points( const Points &p )
//----------------------------------------------------------------------

void BSplineCurve::set_points( const Points &p )
{
	delete points;
	points = new Points( p );
}

//----------------------------------------------------------------------
//	Point BSplineCurve::operator () ( double t ) const
//----------------------------------------------------------------------

Point BSplineCurve::operator () ( double t ) const
{
	TEST_EXPR( base != NULL && points != NULL );
	TEST_EXPR( K() == (uint) points->size() );
	TEST_EXPR( min_t() <= t && t <= max_t() );

	Point	result = 0;
	int	i = 0;
	for( Points::iterator p = points->begin(); p != points->end(); p++ ) {
		result += (*p) * (*base)(i++, t);
	}
	return result;
}

//----------------------------------------------------------------------
//	Point BSplineCurve::derivate( double t, uint k, bool &error ) const
//----------------------------------------------------------------------

Point BSplineCurve::derivate( double t, uint k, bool &error ) const
{
	TEST_EXPR( base != NULL && points != NULL );
	TEST_EXPR( K() == (uint) points->size() );
	TEST_EXPR( min_t() <= t && t <= max_t() );

	Point rv = 0;
	uint i = 0;
	error = false;

	for( Points::iterator p = points->begin(); 
	    p != points->end() && !error; p++ ) {
		rv += (*p) * base->derivate(i++, t, k, error);
	}

	return rv;
}

