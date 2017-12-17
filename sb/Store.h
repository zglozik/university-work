

#ifndef	STORE_H
#define	STORE_H

#include	"stdtypes.h"
#include	"Object.h"
#include	<stdio.h>
class Store : public Object {

    public:

	enum messages { MAKE_FOOD };

	Store( const char *name );

	virtual void handleMessage( const Message &message );

    private:
	void makeFood();
	void Store::parseLine( char *line, char *name,
			      int &number, int &next );

    private:
	FILE	*_finit;
	char	_buf[100];
	bool	_eof;
};


#endif /* STORE_H */
