
#ifndef	MANAGER_H
#define	MANAGER_H

#include	"global.h"
#include	"MessageQueue.h"
#include	<stdio.h>

class Person;

class Manager : public Object {

    public:

	Manager( const char *name, int tid );
	~Manager();

	void handleMessage( const Message &message );

    private:
	void newPerson();
	void diePerson( Person *person );
	void Manager::parseLine( char *line, char *name, int &when );

    private:
	FILE	*_finit;
	bool	_eof;
	char	_buf[100];

};

#endif /* MANAGER_H */
