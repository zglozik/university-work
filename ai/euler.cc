
#include	"euler.hh"
#include	"image.hh"

CellMatrix::CellMatrix( Image *image ) : _image(image)
{
	if( image->width() >= image->height() ) {
		_w = image->width();
		_h = image->height();
	} else {
		_h = image->width();
		_w = image->height();
	}
		
	Cell *_cells1 = new Cell[_w*_h];
	Cell *_cells2 = new Cell[_w*_h];
	_valid_cells = _cells1;
	_temp_cells = _cells2;
}

CellMatrix::~CellMatrix()
{
	delete [] _valid_cells;
	delete [] _temp_cells;
}

int CellMatrix::start()
{
	pass1();
	pass2();
	pass3();

	return result();
}

void CellMatrix::pass1()
{
	for( unsigned i = 0; i < _h; ++i ) {
		for( unsigned j = 0; j < _w; ++j ) {
			set_init_state( i, j );
		}
	}
	change_cells();
}

void CellMatrix::pass2()
{
	for( unsigned step = 0; step < _w-1; ++step ) {
		for( unsigned i = 0; i < _h; ++i ) {
			add_LR_pixel( i, _w-1 );
		}
		for( unsigned i = 0; i < _h; ++i ) {
			for( unsigned j = 0; j < _w-1; ++j ) {
				add_binary_zero( i, j );
			}
		}
		change_cells();
	}
	for( unsigned step = 0; step < _w-1; ++step ) {
		for( unsigned i = 0; i < _h; ++i ) {
			for( unsigned j = 0; j < _w; ++j ) {
				add_binary_zero( i, j );
			}
		}
		change_cells();
	}
}

void CellMatrix::pass3()
{
	for( unsigned step = 0; step < _h-1; ++step ) {
		for( unsigned j = 0; j < _w; ++j ) {
			add_line_below( 0, j );
		}
		for( unsigned i = 1; i < _h; ++i ) {
			for( unsigned j = 0; j < _w; ++j ) {
				shift_line_up( i, j );
			}
		}
		change_cells();
	}
}

int CellMatrix::result()
{
	int	L = 0;
	int	R = 0;
	int	coefficient = 1;
	for( int j = _w-1; j >= 0; --j ) {
		Cell *c = &valid_cell( 0, j );
		L += coefficient * c->L_bit;
		R += coefficient * c->R_bit;
		coefficient *= 2;
	}

	return L - R;	
}

void CellMatrix::set_init_state( int i, int j )
{
	Cell *c = &temp_cell( i, j );
	if( j+1 < int(_w) && i-1 >= 0 ) {
		if( _image->width() >= _image->height() ) {
			c->L_state = _image->get(j,i) && !_image->get(j,i-1)
				&& !_image->get(j+1,i);
			c->R_state = _image->get(j,i) && _image->get(j,i-1)
				&& _image->get(j+1,i) && !_image->get(j+1,i-1);
		} else {
			c->L_state = _image->get(i,j) && !_image->get(i-1,j)
				&& !_image->get(i,j+1);
			c->R_state = _image->get(i,j) && _image->get(i-1,j)
				&& _image->get(i,j+1) && !_image->get(i-1,j+1);
		}
	} else {
		c->L_state = 0;
		c->R_state = 0;
	}
	c->L_bit = 0;
	c->R_bit = 0;
}

void CellMatrix::add_LR_pixel( int i, int j )
{
	Cell *c = &valid_cell( i, j );
	Cell *tc = &temp_cell( i, j );
	Cell *left = neighbour( i, j-1 );

	tc->L_bit = add_binary_digit( c->L_bit, 0, c->L_state );
	tc->L_state = left ? left->L_state : 0;
	tc->R_bit = add_binary_digit( c->R_bit, 0, c->R_state );
	tc->R_state = left ? left->R_state : 0;
}

void CellMatrix::add_binary_zero( int i, int j )
{
	Cell *c = &valid_cell( i, j );
	Cell *tc = &temp_cell( i, j );
	Cell *left = neighbour(i, j-1 );
	Cell *right = neighbour(i, j+1 );

	tc->L_bit = add_binary_digit( c->L_bit, right ? right->L_bit : 0, 0 );
	tc->L_state = left ? left->L_state : 0;
	tc->R_bit = add_binary_digit( c->R_bit, right ? right->R_bit : 0, 0 );
	tc->R_state = left ? left->R_state : 0;
}

void CellMatrix::add_line_below( int i, int j )
{
	Cell *c = &valid_cell( i, j );
	Cell *tc = &temp_cell( i, j );
	Cell *down = neighbour( i+1, j );
	Cell *right = neighbour( i, j+1 );

	tc->L_bit = add_binary_digit( c->L_bit, right ? right->L_bit : 0,
				     down ? down->L_bit : 0 );
	tc->R_bit = add_binary_digit( c->R_bit, right ? right->R_bit : 0,
				     down ? down->R_bit : 0 );
	tc->L_state = c->L_state;
	tc->R_state = c->R_state;
}

void CellMatrix::shift_line_up( int i, int j )
{
	Cell *c = &valid_cell( i, j );
	Cell *tc = &temp_cell( i, j );
	Cell *down = neighbour( i+1, j );

	if( down ) {
		tc->L_bit = down->L_bit;
		tc->R_bit = down->R_bit;
		tc->L_state = c->L_state;
		tc->R_state = c->R_state;
	} else {
		*tc = *c;
	}
}

unsigned CellMatrix::add_binary_digit( unsigned value1, unsigned rvalue,
				      unsigned value2 )
{
	return (value1 & 1u) + (rvalue >> 1 & 1u) + value2;
}

Cell *CellMatrix::neighbour( int i, int j )
{
	if( i < 0 || i >= int(_h) || j < 0 || j >= int(_w) ) return NULL;

	return &valid_cell( i, j );
}

Cell &CellMatrix::valid_cell( int i, int j )
{
	return _valid_cells[ i*_w + j ];
}

Cell &CellMatrix::temp_cell( int i, int j )
{
	return _temp_cells[ i*_w + j ];
}

void CellMatrix::change_cells()
{
	Cell *p = _valid_cells;
	_valid_cells = _temp_cells;
	_temp_cells = p;
}

