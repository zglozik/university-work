
#ifndef	_BODY_HH_
#define	_BODY_HH_

#include	"debug.hh"
#include	"vector.hh"
#include	"array.hh"
#include	"list.hh"
#include	"transform.hh"
#include	"map.hh"
#include	"string.hh"
#include	"viewer.hh"

typedef	Vector<double>		Point;

class BodyBuilder;

class Body {

friend	bool operator == ( const Body &b1, const Body &b2 );
friend	ostream &operator << ( ostream &s, const Body &body );

//	Part class

    public:
	class Part {

	friend	Body;
	friend	BodyBuilder;

	    public:
		Part() { }
		Part( Point *p1, Point *p2, double centre, double weight,
		     bool show_centre )
			: point1(p1), point2(p2), _weight(weight), 
			centre(centre), _show_centre(show_centre) { }

		Point	&start() const	{ return *point1; }
		Point	&end() const	{ return *point2; }
		Point	centre_of_gravity() const {
			return *point1 + centre * (*point2 - *point1);
		}
		double	weight() const	{ return _weight; }
		bool	show_centre() const { return _show_centre; }

	    private:
		Point	*point1;
		Point	*point2;
		double	_weight;
		double	centre; // distance from point1
		bool	_show_centre; // shall we draw the route of the centre?
	}; // Part

//	Wrist class

	class Wrist {

	friend	Body;
	friend	ostream &operator << ( ostream &s, const Body &body );

	    public:
		Wrist() { }
		Wrist( const Part &p1, const Part &p2, const Point &d )
			: part1(p1), part2(p2), direction(normal(d)) { }
	    private:
		Part	part1;
		Part	part2;
		Point	direction;
	};

//	typedefs

	typedef Map< String, Wrist >	WristStore;
	typedef	Map< String, Part >	PartStore;
	typedef	Array<Point>		PointStore;
	typedef	Transform<double>	PointTrans;

//	ctors & dtors

    public:
	Body( PointStore *points, PartStore *parts, WristStore *wrists,
	     bool show_centre, Viewer &viewer );
	Body( Viewer &viewer );
	Body( const Body &body );
virtual	~Body();

//	opers

    public:
	Body	&operator = ( const Body &body );

	const PointStore &points() const { return *_points; }
	const PartStore &parts() const { return *_parts; }
	const WristStore &wrists() const { return *_wrists; }
	bool show_centre() const { return _show_centre; }
	void show_centre( bool sc ) { _show_centre = sc; }

	const Body &draw() const;
	Body	&move( const Point &new_centre_of_gravity );
	Body	&move_rel( const Point &new_centre_of_gravity );
	Point	centre_of_gravity() const;
	double	J( const Point &centre ) const;
	double	weight() const;
	Body	&rotate( double degree, const Point &point );
	Body	&bend( const char *name, double degree );
	Body	&bend_rel( const char *wrist_name, double degree );
	double	alpha( const char *wrist_name ) const;
	double	alpha_horiz( const char *part_name ) const;

//	private data

    private:
	struct	transform_point;

	// axes - direction of p1 x p2 is the same as the direction of axes
	Body		&bend( const Part &p1, const Part &p2, 
			      double degree, Point axes );
	PartStore	*duplicate_parts( const Body &body ) const;
	WristStore	*duplicate_wrists( const Body &body ) const;
	Point		*point_ref( const Body &body, 
				   const Point &point ) const;
	void		walk_body( Point &exclude, Point &start, 
				  const transform_point &op );
	List<Point *>	neighbours( const Point &point ) const;
	bool		is_connected( const Part &p1, const Part &p2,
				     Point *&c, Point *&v1, Point *&v2 ) const;

	struct transform_point {
		transform_point( const PointTrans &trans ) : trans(trans) { }
		Point operator()( const Point &point ) const
		{
			return trans.transform( point );
		}
		PointTrans	trans;
	};

	PointStore	*_points;
	PartStore	*_parts;
	WristStore	*_wrists;
	bool		_show_centre;
	Viewer		&viewer;

};

bool operator == ( const Body &b1, const Body &b2 );
ostream &operator << ( ostream &s, const Body &body );

#endif	// _BODY_HH_
