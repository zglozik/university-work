

#ifndef	STORE_H
#define	STORE_H

#include	"stdtypes.h"
#include	"MessageQueue.h"
#include	<stdio.h>

class Store : public Object {

    public:

	Store( const char *name, int tid );

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
