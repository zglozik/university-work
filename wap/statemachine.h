
#ifndef _STATEMACHINE_H_
#define	_STATEMACHINE_H_

#include	"timer_manager.h"

#include	<sys/time.h>

/* State machine implementation interface file. */

/*
 * State machines are running in one state machine environment of this type.
 */
typedef struct sm_environment_t	sm_environment_t;

typedef struct sm_signal_t	sm_signal_t;

/*
 * The role of this handler is to process incoming signals.
 */
typedef	void	(*sm_signal_handler_t)(sm_signal_t *);

/*
 * Represents the states of the state machines.
 */
typedef int	sm_state_t;

/*
 * This type stores information about one state machine.
 */
typedef struct sm_machine_t {
	sm_environment_t	*env;	/* environment of the machine */
	sm_state_t		state;	/* current state of the machine */
	sm_signal_handler_t	signal_handler;
	void			*user_data;	/* additional user data */
	void			(*destroy_handler)(void *); /* for user_data */
} sm_machine_t;

/*
 * State machines can send information of this type to each other.
 */
struct sm_signal_t {
	sm_machine_t	*destination;	/* who this signal was sent to */
	int		type;		/* signal identifier */
	void		*user_data;	/* signal arguments */
	void		(*destroy_handler)(void *); /* to free user_data */
};

/*
 * State machine environment interface.
 */

/*
 * Creates a new state machine environment object and starts handling
 * incoming signals.
 * This object can run several state machines and dispatch signals
 * to the destination machines.
 */
extern sm_environment_t	*smenv_create();

/*
 * Destroys the specified state machine environment object.
 */
extern void smenv_destroy(sm_environment_t *env);

/*
 * Creates a timer that can be used for sending signals after the
 * specified amount of time.
 */
extern tm_timer_t *smenv_timer_create(time_t sec, long nsec);

/*
 * Destroys a timer.
 */
extern void smenv_timer_destroy(tm_timer_t *timer);

/*
 * Stops a timer. It _must_ be called before the timer expires for the last
 * time.
 */
extern void smenv_timer_stop(int timer_id);

/*
 * Inserts the specified signal into the signal queue.
 */
extern void smenv_send_signal(sm_signal_t *signal);

/*
 * Inserts the specified signal into the signal queue when the specified
 * timer expired.
 *
 * return:	The id of the timer instance. It can be stopped later by
 *		smenv_timer_stop. 
 */
extern int smenv_send_timed_signal(sm_signal_t *signal, tm_timer_t *timer);

/*
 * State machine interface.
 */

extern sm_machine_t *sm_create(sm_environment_t *env,
			       sm_state_t initial_state,
			       sm_signal_handler_t signal_handler,
			       void *user_data,
			       void (*destroy_handler)(void *));

extern void sm_destroy(sm_machine_t *machine);

/*
 * Signal interface.
 */

extern sm_signal_t *sm_signal_create(sm_machine_t *destination, int type,
				     void *user_data,
				     void (*destroy_handler)(void *));

extern void sm_signal_destroy(sm_signal_t *signal);

#endif /* _STATEMACHINE_H_ */
