
#include	<approximation.hh>
#include	<iostream>

int main()
{
	// read knot vector
	cout << "Reading knot vector...\n";
	KnotVector	kv;
	cin >> kv;

	// read parameter domain
	cout << "Reading parameter domain...\n";
	Array<double>	parameters;
	cin >> parameters;

	// read measured data
	cout << "Reading input points...\n";
	Array<Point>	points;
	cin >> points;

	// approximating points
	cout << "Computing curve...\n";

	Approximation	approx( points );
	BSplineCurve	curve;
	approx.init_parameters( kv, parameters );
	double err = approx.approximate1( 3, curve );
	cout << "average error: " << err << endl << endl;

	for( int i = 0; i < parameters.size(); i++ ) {
		cout << "p(" << parameters[i] << ") = ";
		cout << "[ " << curve( parameters[i] ) << " ]\n";
	}
	     
	return 0;
}
