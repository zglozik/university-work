
#ifndef	ASSISTANT_H
#define	ASSISTANT_H

#include	"stdtypes.h"
#include	"MessageQueue.h"
#include	"global.h"

class Person;

class Assistant : public Object {

    public:

	Assistant( const char *name, int tid );

	void handleMessage( const Message &message );

    private:
	void areYouReady( int customer );
	void order( Food what, int number );
	void enough();
	void giveMoney( int money );
	void addFood( Food what, int number );

    private:
	int	_customer;
	int	_bill;
	int	food_quantity[NUM_FOOD];

};

#endif /* ASSISTANT_H */
