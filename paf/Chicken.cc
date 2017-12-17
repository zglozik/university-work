
#include	"Chicken.h"

void Chicken::eat()
{
	Animal::eat();
	print( "Kaptam enni buzadarat, ezen elragcsalok egy kicsit." );
}

int main()
{
	MainLoop<Chicken> a;

	a();
}
