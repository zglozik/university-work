
#include	"Pig.h"

void Pig::eat()
{
	Animal::eat();
	print( "Kaptam enni moslekot, finom." );
}

int main()
{
	MainLoop<Pig> a;

	a();
}
