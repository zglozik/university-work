
#include	"statemachine.h"
#include	"debug.h"
#include	"queue.h"
#include	"timer_manager.h"

#include	<assert.h>
#include	<pthread.h>
#include	<stdlib.h>

/*
 * This object stores information about the state machines that are
 * running in the same environment, that is, who can send signals
 * to each other using the services of the environment.
 */
struct sm_environment_t {
	pthread_t	handler_thread;
	queue_t		*signal_queue;
	pthread_mutex_t	queue_mutex;	/* mutex for accessing signal_queue */
	pthread_cond_t	queue_cond;	/* to signal when new signal arrives */
};

/*
 * Private function declarations.
 */

/*
 * When invoked places its signal argument into the signal queue of the
 * state machine environment. client_data must be of type sm_signal_t *.
 */
static void smenv_timer_handler(int id, void *client_data);

/*
 * Main handler function of the state machine environment thread.
 */
static void *smenv_thread_main(void *);

/*
 * Function definitions.
 */

extern sm_environment_t	*smenv_create()
{
	sm_environment_t *env = 
		(sm_environment_t *) malloc(sizeof(sm_environment_t));
	int err;

	if (env == NULL)
		return NULL;
	env->signal_queue = queue_create(100);
	if (env->signal_queue == NULL) {
		free(env);
		return NULL;
	}
	pthread_mutex_init(&env->queue_mutex, NULL);
	pthread_cond_init(&env->queue_cond, NULL);

	err = pthread_create(&env->handler_thread, NULL,
			     smenv_thread_main, env);
	if (err != 0) {
		queue_destroy(env->signal_queue);
		free(env);
		return NULL;
	}

	return env;
}

extern void smenv_destroy(sm_environment_t *env)
{
	assert(env != NULL);
	
	pthread_join(env->handler_thread, NULL);

	queue_destroy(env->signal_queue);
	pthread_mutex_destroy(&env->queue_mutex);
	pthread_cond_destroy(&env->queue_cond);
	free(env);
}

/*
 * client_data must be of type sm_environment_t *.
 */
static void *smenv_thread_main(void *client_data)
{
	sm_environment_t *env = (sm_environment_t *) client_data;
	
	assert(env != NULL);

	while (1) {
		sm_signal_t	*signal;

		pthread_mutex_lock(&env->queue_mutex);
		while (queue_empty(env->signal_queue)) {
			pthread_cond_wait(&env->queue_cond, &env->queue_mutex);
		}
		signal = (sm_signal_t *) queue_pop(env->signal_queue);
		pthread_mutex_unlock(&env->queue_mutex);

		if (signal != NULL) {
			/* call the handler of the destination state machine */
			(*signal->destination->signal_handler)(signal);
			sm_signal_destroy(signal);
		}
	}
}

extern tm_timer_t *smenv_timer_create(time_t sec, long nsec)
{
	return tm_create_timer(sec, nsec, smenv_timer_handler);
}

extern void smenv_timer_destroy(tm_timer_t *timer)
{
	tm_destroy_timer(timer);
}

extern void smenv_timer_stop(int timer_id)
{
	tm_stop_timer(timer_id);
}

extern void smenv_send_signal(sm_signal_t *signal)
{
	sm_environment_t	*env;
	queue_t			*queue;
	int	err;

	assert(signal != NULL);
	env = signal->destination->env;
	queue = env->signal_queue;

	pthread_mutex_lock(&env->queue_mutex);
	err = queue_add(queue, signal);
	assert(err == 0);
	pthread_cond_signal(&env->queue_cond);
	pthread_mutex_unlock(&env->queue_mutex);
}

/*
 * client_data must be of type sm_signal_t *.
 */
static void smenv_timer_handler(int id, void *client_data)
{
	sm_signal_t *signal = (sm_signal_t *) client_data;

	assert(signal != NULL);
	smenv_send_signal(signal);
}

extern int smenv_send_timed_signal(sm_signal_t *signal, tm_timer_t *timer)
{
	assert(signal != NULL);
	assert(timer != NULL);

	return tm_start_timer(timer, signal, 1);
}

extern sm_machine_t *sm_create(sm_environment_t *env,
			       sm_state_t initial_state,
			       sm_signal_handler_t signal_handler,
			       void *user_data,
			       void (*destroy_handler)(void *))
{
	sm_machine_t *machine = (sm_machine_t *) malloc(sizeof(sm_machine_t));
	if (machine == NULL)
		return NULL;

	machine->env = env;
	machine->state = initial_state;
	assert(signal_handler != NULL);
	machine->signal_handler = signal_handler;
	machine->user_data = user_data;
	machine->destroy_handler = destroy_handler;

	return machine;
}

extern void sm_destroy(sm_machine_t *machine)
{
	assert(machine != NULL);

	if (machine->user_data != NULL && machine->destroy_handler != NULL)
		(*machine->destroy_handler)(machine->user_data);

	free(machine);
}

extern sm_signal_t *sm_signal_create(sm_machine_t *destination, int type,
				     void *user_data,
				     void (*destroy_handler)(void *))
{
	sm_signal_t *signal = (sm_signal_t *) malloc(sizeof(sm_signal_t));
	if (signal == NULL)
		return NULL;

	signal->destination = destination;
	signal->type = type;
	signal->user_data = user_data;
	signal->destroy_handler = destroy_handler;
	
	return signal;
}

extern void sm_signal_destroy(sm_signal_t *signal)
{
	assert(signal != NULL);
	
	if (signal->user_data != NULL &&
	    signal->destroy_handler != NULL) {
		(*signal->destroy_handler)(signal->user_data);
	}
	free(signal);
}	

/*
 * Test functions.
 */

enum trspt_machine_signals_t {
	TRSPT_INVOKE_REQ, TRSPT_INVOKE_IND, TRSPT_INVOKE_RES, TRSPT_INVOKE_CNF
};

typedef struct trspt_invoke_req_arg_t {
	sm_machine_t	*source;	/* source wtp_machine */
	sm_machine_t	*destination;	/* destination wtp_machine */
	void		*message;
	int		message_len;
} trspt_invoke_req_arg_t;

typedef trspt_invoke_req_arg_t	trspt_invoke_ind_arg_t;

typedef trspt_invoke_req_arg_t	trspt_invoke_res_arg_t;

typedef trspt_invoke_req_arg_t	trspt_invoke_cnf_arg_t;

static sm_machine_t *trspt_machine_create(sm_environment_t *env);
static void trspt_machine_destroy(void *user_data);
static void trspt_invoke_req_arg_destroy(void *user_data);
static void trspt_invoke_ind_arg_destroy(void *user_data);
static void trspt_invoke_res_arg_destroy(void *user_data);
static void trspt_invoke_cnf_arg_destroy(void *user_data);
static void trspt_handler_accept_req_trspt_invoke_req(sm_signal_t *signal);
static void trspt_handler_accept_res_trspt_invoke_res(sm_signal_t *signal);
static void trspt_handler(sm_signal_t *signal);

enum wtp_machine_signals_t {
	WTP_INVOKE_REQ
};

typedef struct wtp_invoke_req_arg_t {
	sm_machine_t	*destination;		/* peer wtp machine */
	char		message[100];
} wtp_invoke_req_arg_t;

static void wtp_invoke_req_arg_destroy(void *user_data);
static sm_machine_t *wtp_machine_create(sm_environment_t *env,
					sm_machine_t *provider);
static void wtp_machine_destroy(void *user_data);
static void wtp_handler_init_wtp_invoke_req(sm_signal_t *signal);
static void wtp_handler_init_trspt_invoke_ind(sm_signal_t *signal);
static void wtp_handler_accept_cnf_trspt_invoke_cnf(sm_signal_t *signal);
static void wtp_handler(sm_signal_t *signal);

/*
 * wtp machine
 */

enum wtp_machine_states_t {
	WTP_INIT, WTP_ACCEPT_CNF
};

typedef struct wtp_machine_t {
	sm_machine_t	*provider;
} wtp_machine_t;

static void wtp_invoke_req_arg_destroy(void *user_data)
{
	wtp_invoke_req_arg_t *arg = (wtp_invoke_req_arg_t *) user_data;

	free(arg);
}

static sm_machine_t *wtp_machine_create(sm_environment_t *env,
					sm_machine_t *provider)
{
	sm_machine_t	*machine;
	wtp_machine_t	*wtp_machine;

	wtp_machine = (wtp_machine_t *) malloc(sizeof(wtp_machine_t));
	wtp_machine->provider = provider;
	
	machine = sm_create(env, WTP_INIT, wtp_handler,
			    wtp_machine, wtp_machine_destroy);
	return machine;
}

static void wtp_machine_destroy(void *user_data)
{
	wtp_machine_t	*wtp_machine = (wtp_machine_t *) user_data;
	
	free(wtp_machine);
}

static void wtp_handler_init_wtp_invoke_req(sm_signal_t *signal)
{
	sm_machine_t 	*machine = signal->destination;
	wtp_machine_t	*wtp_machine = (wtp_machine_t *) machine->user_data;
	wtp_invoke_req_arg_t	*arg =
		(wtp_invoke_req_arg_t *) signal->user_data;
	trspt_invoke_req_arg_t	*req_arg;
	void			trspt_invoke_req_arg_destroy(void *user_data);

	PRINTF(("message: %s\n", arg->message));

	req_arg = (trspt_invoke_req_arg_t *)
		malloc(sizeof(trspt_invoke_req_arg_t));
	req_arg->source = machine;
	req_arg->destination = arg->destination;
	req_arg->message_len = sizeof(char) * (strlen(arg->message) + 1);
	req_arg->message = malloc(req_arg->message_len);
	memcpy(req_arg->message, arg->message, req_arg->message_len);

	signal = sm_signal_create(wtp_machine->provider, TRSPT_INVOKE_REQ,
				  req_arg, trspt_invoke_req_arg_destroy);
	smenv_send_signal(signal);

	machine->state = WTP_ACCEPT_CNF;
}

static void wtp_handler_init_trspt_invoke_ind(sm_signal_t *signal)
{
	sm_machine_t 	*machine = signal->destination;
	wtp_machine_t	*wtp_machine = (wtp_machine_t *) machine->user_data;
	trspt_invoke_ind_arg_t	*arg =
		(trspt_invoke_ind_arg_t *) signal->user_data;
	trspt_invoke_res_arg_t	*res_arg;

	PRINTF(("message: %s\n", (const char *) arg->message));

	res_arg = (trspt_invoke_res_arg_t *)
		malloc(sizeof(trspt_invoke_res_arg_t));
	res_arg->source = machine;
	res_arg->destination = arg->source;
	res_arg->message = malloc(sizeof(int));
	*(int *) res_arg->message = -1 * arg->message_len;
	res_arg->message_len = sizeof(int);

	signal = sm_signal_create(wtp_machine->provider, TRSPT_INVOKE_RES,
				  res_arg, trspt_invoke_res_arg_destroy);
	smenv_send_signal(signal);
}

static void wtp_handler_accept_cnf_trspt_invoke_cnf(sm_signal_t *signal)
{
	sm_machine_t 	*machine = signal->destination;
	trspt_invoke_cnf_arg_t	*arg =
		(trspt_invoke_cnf_arg_t *) signal->user_data;

	PRINTF(("message received: %d\n", *(int *) arg->message));

	machine->state = WTP_INIT;
}

static void wtp_handler(sm_signal_t *signal)
{
	sm_machine_t *machine = signal->destination;

	switch(machine->state) {
	case WTP_INIT:
		switch(signal->type) {
		case WTP_INVOKE_REQ:
			wtp_handler_init_wtp_invoke_req(signal);
			break;
		case TRSPT_INVOKE_IND:
			wtp_handler_init_trspt_invoke_ind(signal);
			break;
		default:
			PRINTF(("signal ignored\n"));
		}
		break;
	case WTP_ACCEPT_CNF:
		switch(signal->type) {
		case TRSPT_INVOKE_CNF:
			wtp_handler_accept_cnf_trspt_invoke_cnf(signal);
			break;
		default:
			PRINTF(("signal ignored\n"));
		}
		break;
	default:
		;
	}
}

/*
 * Transport machine.
 */

enum trspt_machine_states_t {
	TRSPT_ACCEPT_REQ, TRSPT_ACCEPT_RES
};

typedef struct trspt_machine_t {
	tm_timer_t	*timer;		/* it will send the message after 1s */
} trspt_machine_t;

static sm_machine_t *trspt_machine_create(sm_environment_t *env)
{
	sm_machine_t	*machine;
	trspt_machine_t *trspt_machine =
		(trspt_machine_t *) malloc(sizeof(trspt_machine_t));

	trspt_machine->timer = smenv_timer_create(1, 0l);

	machine = sm_create(env, TRSPT_ACCEPT_REQ, trspt_handler,
			    trspt_machine, trspt_machine_destroy);

	return machine;
}

static void trspt_machine_destroy(void *user_data)
{
	trspt_machine_t *trspt_machine = (trspt_machine_t *) user_data;

	smenv_timer_destroy(trspt_machine->timer);
	free(trspt_machine);
}

static void trspt_invoke_req_arg_destroy(void *user_data)
{
	trspt_invoke_req_arg_t *arg = (trspt_invoke_req_arg_t *) user_data;

	free(arg->message);
	free(arg);
}

static void trspt_invoke_ind_arg_destroy(void *user_data)
{
	trspt_invoke_req_arg_destroy(user_data);
}

static void trspt_invoke_res_arg_destroy(void *user_data)
{
	trspt_invoke_req_arg_destroy(user_data);
}

static void trspt_invoke_cnf_arg_destroy(void *user_data)
{
	trspt_invoke_req_arg_destroy(user_data);
}

static void trspt_handler_accept_req_trspt_invoke_req(sm_signal_t *signal)
{
	sm_machine_t 	*machine = signal->destination;
	trspt_machine_t	*trspt_machine = 
		(trspt_machine_t *) machine->user_data;
	trspt_invoke_req_arg_t	*arg =
		(trspt_invoke_req_arg_t *) signal->user_data;
	trspt_invoke_ind_arg_t	*ind_arg;

	PRINTF(("message received\n"));

	ind_arg = (trspt_invoke_ind_arg_t *)
		malloc(sizeof(trspt_invoke_ind_arg_t));
	ind_arg->source = arg->source;
	ind_arg->destination = arg->destination;
	ind_arg->message = malloc(arg->message_len);
	memcpy(ind_arg->message, arg->message, arg->message_len);
	ind_arg->message_len = arg->message_len;

	signal = sm_signal_create(ind_arg->destination, TRSPT_INVOKE_IND,
				  ind_arg, trspt_invoke_ind_arg_destroy);
	smenv_send_timed_signal(signal, trspt_machine->timer);

	machine->state = TRSPT_ACCEPT_RES;
}

static void trspt_handler_accept_res_trspt_invoke_res(sm_signal_t *signal)
{
	sm_machine_t 	*machine = signal->destination;
	trspt_machine_t	*trspt_machine = 
		(trspt_machine_t *) machine->user_data;
	trspt_invoke_res_arg_t	*arg =
		(trspt_invoke_res_arg_t *) signal->user_data;
	trspt_invoke_cnf_arg_t	*cnf_arg;

	PRINTF(("message received\n"));

	cnf_arg = (trspt_invoke_cnf_arg_t *)
		malloc(sizeof(trspt_invoke_cnf_arg_t)); 
	cnf_arg->source = arg->source;
	cnf_arg->destination = arg->destination;
	cnf_arg->message = malloc(arg->message_len);
	memcpy(cnf_arg->message, arg->message, arg->message_len);
	cnf_arg->message_len = arg->message_len;

	signal = sm_signal_create(cnf_arg->destination, TRSPT_INVOKE_CNF,
				  cnf_arg, trspt_invoke_cnf_arg_destroy);
	smenv_send_timed_signal(signal, trspt_machine->timer);

	machine->state = TRSPT_ACCEPT_REQ;
}

static void trspt_handler(sm_signal_t *signal)
{
	sm_machine_t *machine = signal->destination;

	switch(machine->state) {
	case TRSPT_ACCEPT_REQ:
		switch(signal->type) {
		case TRSPT_INVOKE_REQ:
			trspt_handler_accept_req_trspt_invoke_req(signal);
			break;
		default:	
			PRINTF(("signal ignored\n"));
		}
		break;
	case TRSPT_ACCEPT_RES:
		switch(signal->type) {
		case TRSPT_INVOKE_RES:
			trspt_handler_accept_res_trspt_invoke_res(signal);
			break;
		default:	
			PRINTF(("signal ignored\n"));
		}
		break;
	default:
		assert(1);
	}
}

static wtp_invoke_req_arg_t *wtp_invoke_req_arg_create(sm_machine_t *dest,
						       char *message)
{
	wtp_invoke_req_arg_t	*req_arg;
	req_arg =
		(wtp_invoke_req_arg_t *) malloc(sizeof(wtp_invoke_req_arg_t));
	req_arg->destination = dest;
	strcpy(req_arg->message, message);

	return req_arg;
}

/*
 * Functional test main function.
 */
static void functional_test()
{
	sm_environment_t *env;
	sm_machine_t	 *wtp1;
	sm_machine_t	 *wtp2;
	sm_machine_t	 *trspt;
	wtp_invoke_req_arg_t	*req_arg;
	sm_signal_t	 *signal;

	tm_init();

	env = smenv_create();
	assert(env != NULL);

	trspt = trspt_machine_create(env);
	wtp1 = wtp_machine_create(env, trspt);
	wtp2 = wtp_machine_create(env, trspt);
	assert(trspt != NULL && wtp1 != NULL && wtp2 != NULL);

	req_arg = wtp_invoke_req_arg_create(wtp2, "hi");
	signal = sm_signal_create(wtp1, WTP_INVOKE_REQ,
				  req_arg, wtp_invoke_req_arg_destroy);
	smenv_send_signal(signal);

	smenv_destroy(env);
}

int main()
{
	functional_test();

	return 0;
}
