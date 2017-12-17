
#ifndef	BUYER_H
#define	BUYER_H

#include	"Object.h"

class Buyer : public Object {

    public:
	enum Messages { BUY };

	Buyer();

	void handle_message( const Message &message );

    private:

	void buy();

};

#endif /* BUYER_H */
