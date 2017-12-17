
#ifndef	SB_H
#define	SB_H

#include	"Object.h"
#include	"Timer.h"
#include	"Manager.h"
#include	"WaitingQueue.h"
#include	"Assistant.h"
#include	"Store.h"

extern Timer		*timer;
extern Queue< Message >	*message_queue;
extern Manager		*manager;
extern WaitingQueue	*waiting_queue;
extern Assistant	*assistant;
extern Store		*store;

#endif /* SB_H */
