
#ifndef	BUYER_H
#define	BUYER_H

#include	"Object.h"

class Buyer : public Object {

    public:
	Buyer( char *name, int tid );

	void handle_message( const Message &message );

    private:

	void buy();

};

#endif /* BUYER_H */
