
#include	<iomanip.h>
#include	<fstream.h>
#include	<math.h>
#include	"array.hh"
#include	"list.hh"

#define	LINE_SEP	'#'
#define	FIELD_SEP	','

typedef	List< Array<double> >	Chart;

// private functions

static bool read_chart( Chart &chart, istream &str );
static double compute_k( const Chart &chart, int j, double time_slice );
static double compute_function( double p );
static void do_job( const Chart &chart, double time_slice );

//----------------------------------------------------------------------
//	InputFile
//----------------------------------------------------------------------

class InputFile {

    protected:
	InputFile( const InputFile & );
	InputFile &operator = ( const InputFile & );

    public:
	typedef	Array<double>	Data;

	InputFile( istream &str );

	bool read( bool &tsx, Data &tdx );

    private:
	bool	sx;
	bool	state;
	double	number;
	istream	str;
};

InputFile::InputFile( istream &str ) : str(str)
{
	sx = str >> number;
	state = str.eof() || str.good();
}

bool InputFile::read( bool &tsx, Data &tdx )
{
	bool ok = state;

	tsx = sx;
	if( sx && ok ) {
		char c;
		ok = str >> c;
		while( ok && c != FIELD_SEP && c != LINE_SEP ) ok = str >> c;
		
		if( ok ) {
			tdx.resize( 0 );
			tdx.push_back( number );

			while( ok && c == FIELD_SEP ) {
				ok = str >> number;
				if( ok ) {
					tdx.push_back( number );
					ok = str >> c;
					while( ok && c != FIELD_SEP &&
					      c != LINE_SEP )
						ok = str >> c;
				}
			}
			sx = str >> number;
			state = str.eof() || str.good();
		}
	}
	return ok;
}

//----------------------------------------------------------------------
//	bool read_chart( Chart &chart, istream &str );
// ----------------------------------------------------------------------

static bool read_chart( Chart &chart, istream &str )
{
	InputFile	input( str );
	InputFile::Data	dx;
	bool		sx, ok;

	ok = input.read( sx, dx );
	while( ok && sx ) {
//		copy( dx.begin(), dx.end(), 
//		     ostream_iterator<double>(cout, ", ") );
//		cout << "\b\b \n";
		chart.push_back( dx );
		ok = input.read( sx, dx );
	}
	return ok && chart.size() > 1 && chart.front().size() > 0;
}

//----------------------------------------------------------------------
//	static double compute_k( const Chart &chart, int j, double time_slice )
//----------------------------------------------------------------------

static double compute_k( const Chart &chart, int j, double time_slice )
{
	double i = 0;
	Chart::iterator prev, current;

	prev = current = chart.begin();
	current++;
	for( ; current != chart.end(); prev = current, current++ ) {
		i += ((*prev)[j] + (*current)[j]) * time_slice / 2;
	}
	if( i <= 0 ) {
		cerr << "bad input, zero integral\n";
		exit( 1 );
	}

	return 1 / i;
}

//----------------------------------------------------------------------
//	compute_entropy( const Chart &chart, int j, 
//			double time_slice, double k );
//----------------------------------------------------------------------

static double compute_entropy( const Chart &chart, int j, 
			      double time_slice, double k )
{
	double	entropy = 0;
	Chart::iterator current = chart.begin();
	double prevp = compute_function( k * (*current)[j] );

	for( current++; current != chart.end(); current++ ) {
		double p = compute_function( k * (*current)[j] );

		entropy -= (prevp + p) * time_slice / 2;
	}

	return entropy;
}

static double compute_function( double p )
{
	double value = 0;

	if( p < 0 ) {
		cerr << "bad probability: " << p << endl;
		exit(1);
	} else if( p > 0 ) {
		value = p * log(p) / log(2);
	}

	return value;
}

//----------------------------------------------------------------------
//	static void do_job( const Chart &chart, double time_slice );
//----------------------------------------------------------------------

static void do_job( const Chart &chart, double time_slice )
{
	int width = chart.front().size();
	Array<double> entropies( width );
	double entropy = 0;

	setprecision( 10 );
	for( int j = 0; j < width; j++ ) {
		double k = compute_k( chart, j, time_slice );
		entropies[j] = compute_entropy( chart, j, time_slice, k );
		cout << j+1 << ". entropy: " << entropies[j] << endl;
		entropy += entropies[j];
	}
	cout << "\nsum entropy: " << entropy << endl;
}

//----------------------------------------------------------------------
//	main
//----------------------------------------------------------------------

int main( int argc, char *argv[] )
{
	Chart		chart;
	double		time_slice;
	ifstream	input_file;
	istream		*input;

	if( argc != 3 ) {
		cerr << "usage: entropy frequency file_name" << endl;
		exit( 1 );
	} else {
		time_slice = atoi(argv[1]);
		if( time_slice <= 0 ) {
			cerr << "bad frequency" << endl;
			exit( 1 );
		}
		time_slice = 1/time_slice;
		if( !strcmp(argv[2],"-") ) {
			input = &cin;
		} else {
			input_file.open( argv[2] );
			if( !input_file ) {
				cerr << "can't open file: " << argv[2] << endl;
				exit(1);
			}
			input = &input_file;
		}
	}
	if( !read_chart( chart, *input ) ) {
		cerr << "error occured during processing of input file\n";
		exit( 1 );
	}

	do_job( chart, time_slice );

	return 0;
}
