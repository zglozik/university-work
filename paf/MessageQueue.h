
#ifndef	MESSAGEQUEUE_H
#define	MESSAGEQUEUE_H

#include	"Object.h"

class MessageQueue {

    public:
	void in( const Message &message );
	Message *out();
};

#endif /* MESSAGEQUEUE_H */
