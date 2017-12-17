
#ifndef	ASSISTANT_H
#define	ASSISTANT_H

#include	"stdtypes.h"
#include	"Object.h"

class Person;

#define	NUM_FOOD	7

enum Food {
	HAMBURGER, CHICKEN, PIZZA, COKE, SPRITE, FANTA, TONIC
};

extern char	*food_names[];
extern int	food_prices[];

class Assistant : public Object {

    public:
	enum messages {
		ARE_YOU_READY, ORDER, ENOUGH, GIVE_MONEY,
		ADD_FOOD
	};

	struct orderParam {
		orderParam( Food what, int number )
		{
			_what = what;
			_number = number;
		}

		Food	_what;
		int	_number;
	};

	struct addFoodParam {
		addFoodParam( Food what, int number )
		{
			_what = what;
			_number = number;
		}

		Food	_what;
		int	_number;
	};

	Assistant( const char *name );

	void handleMessage( const Message &message );

    private:
	void areYouReady( Person *customer );
	void order( Food what, int number );
	void enough();
	void giveMoney( int money );
	void addFood( Food what, int number );

    private:
	Person	*_customer;
	int	_bill;
	int	food_quantity[NUM_FOOD];

};

#endif /* ASSISTANT_H */
