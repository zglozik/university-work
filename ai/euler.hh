
#ifndef	_EULER_HH_
#define	_EULER_HH_

class Image;

struct Cell {
	unsigned	L_state:1, R_state:1;
	unsigned	L_bit:2, R_bit:2;
};

class CellMatrix {

    public:
	CellMatrix( Image *image );
	~CellMatrix();

	int	start();

    private:
	void	pass1();
	void	pass2();
	void	pass3();
	void	pass4();

	// pass 1
	void	set_init_state( int i, int j );

	// pass 2
	void	add_LR_pixel( int i, int j );
	void	add_binary_zero( int i, int j );

	// pass 3
	void	add_line_below( int i, int j );
	void	shift_line_up( int i, int j );

	// compute difference
	int	result();

	// common members
	unsigned add_binary_digit( unsigned value1, unsigned rvalue,
				  unsigned value2 );
	Cell	*neighbour( int i, int j );
	Cell	&valid_cell( int i, int j );
	Cell	&temp_cell( int i, int j );
	void	change_cells();

    private:
	unsigned _w, _h;

	Cell	*_valid_cells;
	Cell	*_temp_cells;
	Image	*_image;
};

#endif	// _EULER_HH_
