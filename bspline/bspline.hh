
#ifndef	_BSPLINE_HH_
#define	_BSPLINE_HH_

#include	<stdtypes.h>
#include	<array.hh>
#include	<list.hh>
#include	<vector.hh>
#include	<angles.hh>
#include	<map.hh>
#include	<matrix.hh>

typedef Array<double>	KnotVector;
typedef	Vector<double>	Point;
typedef	Array<Point>	Points;

class BSplineCurve;

class Base {

friend	ostream &operator << ( ostream &o, const Base &b );
friend	istream &operator >> ( istream &i, Base &b );

    public:
	Base() : kv(0), n(0) { }
	Base( uint n, const KnotVector &kv );
	Base( uint n, uint k, double t0 = 0.0, double t1 = 1.0 );

	double	operator () ( uint i, double t ) const;
	double	derivate( uint i, double t, uint k, bool &error ) const;

	double	min_t() const;	// minimum parameter value to evaluate
	double	max_t() const;	// maximum parameter value to evaluate
	uint	K() const;	// number of base functions

    private:
	double	compute( uint n, uint i, double t ) const;
	double	compute_derivative( uint i, uint n, double t, uint k ) const;

	void	update_m( const KnotVector &kv );

	typedef	Map<double,uint> Multis;

	KnotVector	kv;	// knot parameter values
	Multis		m;	// multiplicity of knots
	uint		n;	// degree of base function
};

class BSplineCurve {

    public:
	BSplineCurve() : base(0), points(0) { }
	BSplineCurve( const BSplineCurve & b );
virtual	~BSplineCurve();

	void	set_param( uint n, const KnotVector &kv );
	void	set_param( uint n, uint k, double t0 = 0.0, double t1 = 1.0 );
	void	set_points( const Points &p );

	Points	&get_points() const { return *points; }

	Point	operator () ( double t ) const;
	Point	derivate( double t, uint k, bool &error ) const;

	const Base	&ubase() const { return *base; }
	double	min_t() const { return base->min_t(); }
	double	max_t() const { return base->max_t(); }
	uint	K() const { return base->K(); }

    private:
	Base	*base;
	Points	*points;
};

#endif // _BSPLINE_HH_
