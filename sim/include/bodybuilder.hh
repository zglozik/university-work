
#ifndef	_BODY_BUILDER_HH_
#define	_BODY_BUILDER_HH_

#include	"body.hh"

class BodyBuilder {

//	typedefs

    public:
	enum Endpoint {
		start, end
	};

//	ctors & dtor

    private:
	BodyBuilder( const BodyBuilder & );
	BodyBuilder &operator = ( const BodyBuilder & );
    public:
	BodyBuilder( Viewer &viewer );
virtual	~BodyBuilder();

//	opers

    public:
	bool	add_part( const String &name, double length,
			 double weight, double centre, bool show_centre );
	bool	add_wrist( const String &name,
			  const String &name1, Endpoint p1,
			  const String &name2, Endpoint p2,
			  const Point &dir );
	bool	init_wrist( const String &name, double angle );
	bool	init( const String &name, Endpoint ep,
		     const Point &point, double angle );
	Body	*operator () ( bool show_centre );

	const char *error() { return error_str; }

//	private data

    private:

	struct TPart {
		TPart() { }
		TPart( double length, double weight, double centre,
		      bool show_centre, const Body::PartStore::iterator &pos )
			: length(length), weight(weight), centre(centre),
			show_centre(show_centre), pos(pos)  { }
		double	length;
		double	weight;
		double	centre;
		bool	show_centre;
		Body::PartStore::iterator pos;
	};

	Point	&endpoint( const Body::Part &part, Endpoint ep );
	void	push_tpart( const String &pname, Point &fixed, 
			   Endpoint ep, TPart &tpart );

	typedef	Map<String, TPart>	TPartStore;

	TPartStore		tparts;
	
	Body::PointStore	*points;
	Body::PartStore		*parts;
	Body::WristStore	*wrists;
	Viewer			&viewer;

	Body	*body;
	char	error_str[200];
};


#endif // _BODY_BUILDER_HH_
