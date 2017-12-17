
#ifndef	MANAGER_H
#define	MANAGER_H

#include	"Object.h"
#include	<stdio.h>

class Person;

class Manager : public Object {

    public:
	enum { NEW_PERSON, DIE_PERSON };

	Manager( const char *name );
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
