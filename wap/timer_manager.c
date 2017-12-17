
#include	"timer_manager.h"

#include	<unistd.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<pthread.h>

struct tm_timer_t {
	struct timespec	interval;
	tm_handler_t	handler;
};

typedef struct tm_active_node_t {
	int		id;		/* unique active node id */
	tm_timer_t	*parent;
	void		*client_data;
	unsigned	activation;	/* how many times should we run the
					   timer? -1 means always. */
	struct timespec	alarm_time;
} tm_active_node_t;

static tm_active_node_t	*tm_active_nodes = NULL;

#define	TM_INVALID_INDEX	-1
static int		*tm_id2index_map = NULL;
static int		tm_current_id;

static int		tm_active_nodes_length = 0;
static int		tm_max_active_nodes = 2;

static int		min_node_changed;

static int		tm_main_quit;

static pthread_t	tm_main_thread;

static pthread_mutex_t	nodes_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t	alarm_cond = PTHREAD_COND_INITIALIZER;

/* private function declarations */

static int	tm_active_nodes_empty();
static tm_active_node_t	*tm_active_nodes_min();
static int	tm_add_active_node(tm_timer_t *parent, void *client_data,
				   unsigned activation, int id_hint);
static void	tm_set_active_node(tm_active_node_t *node, int new_index);
static int	tm_remove_active_node(unsigned id);
static int	tm_parent_active_node(unsigned current);
static int	tm_left_active_node(unsigned current);
static int	tm_right_active_node(unsigned current);
static int	tm_next_less_node(unsigned current);
static void	tm_descend_active_node(unsigned current);
static void	tm_ascend_active_node(unsigned current);
static int	tm_cmp_active_node(tm_active_node_t *a, tm_active_node_t *b);
static void	*tm_main(void *data);

/* function definitions */

static int tm_active_nodes_empty()
{
	return tm_active_nodes_length == 0;
}

static tm_active_node_t	*tm_active_nodes_min()
{
	return &tm_active_nodes[0];
}

/*
 * id_hint: hint for the id, or TM_INVALID_INDEX.
 * return: id of the new node.
 */
static int tm_add_active_node(tm_timer_t *parent, void *client_data,
			      unsigned activation, int id_hint)
{
	tm_active_node_t	new_node;
	tm_active_node_t	old_first_node;
	struct timeval		now;
	unsigned long		usec;

	if (tm_active_nodes_length == tm_max_active_nodes) {
		int i;
		int new_size = tm_max_active_nodes * 1.5;
		tm_active_node_t *new_nodes = (tm_active_node_t *)
			realloc(tm_active_nodes,
				sizeof(tm_active_node_t) * new_size);
		int *new_map = (int *)
			realloc(tm_id2index_map, sizeof(int) * new_size);
		if (new_nodes == NULL || new_map == NULL) {
			fprintf(stderr, "can't add active timer\n");
			return TM_INVALID_INDEX;
		}
		for (i = tm_max_active_nodes; i < new_size; ++i ) {
			new_map[i] = TM_INVALID_INDEX;
		}
		tm_max_active_nodes = new_size;
		tm_active_nodes = new_nodes;
		tm_id2index_map = new_map;
	}

	if (!tm_active_nodes_empty())
		old_first_node = *tm_active_nodes_min();

	/* There is at least one TM_INVALID_INDEX. */
	if (id_hint != TM_INVALID_INDEX)
		tm_current_id = id_hint;
	while (tm_id2index_map[tm_current_id] != TM_INVALID_INDEX) {
		tm_current_id = (tm_current_id + 1) % tm_max_active_nodes;
	}

	new_node.id = tm_current_id;
	new_node.parent = parent;
	new_node.client_data = client_data;
	new_node.activation = activation;
	gettimeofday(&now, NULL);
	usec = now.tv_usec * 1000ul + parent->interval.tv_nsec;
	new_node.alarm_time.tv_sec = now.tv_sec + parent->interval.tv_sec
		+ usec / 1000000000ul;
	new_node.alarm_time.tv_nsec = usec % 1000000000ul;
	tm_set_active_node(&new_node, tm_active_nodes_length++);
	tm_ascend_active_node(tm_active_nodes_length - 1);

	return tm_current_id;
}

static void tm_set_active_node(tm_active_node_t *node, int new_index)
{
	tm_active_nodes[new_index] = *node;
	tm_id2index_map[node->id] = new_index;
}

/*
 * return: true if the minimal element was changed.
 */
static int tm_remove_active_node(unsigned id)
{
	int index = tm_id2index_map[id];
	if (index == TM_INVALID_INDEX)
		return 0;

	tm_id2index_map[id] = TM_INVALID_INDEX;
	if (--tm_active_nodes_length == index)
		return index == 0;

	/* move the last node to the indexth position */
	tm_set_active_node(&tm_active_nodes[tm_active_nodes_length], index);
	
	tm_descend_active_node(index);

	return index == 0;
}

static int tm_parent_active_node(unsigned current)
{
	return (int) current == 0 ? -1 : (current + 1) / 2 - 1;
}

static int tm_left_active_node(unsigned current)
{
	int left = 2 * (current + 1) - 1;
	return left < tm_active_nodes_length ? left : - 1;
}

static int tm_right_active_node(unsigned current)
{
	int right = 2 * (current + 1) - 1 + 1;
	return right < tm_active_nodes_length ? right : - 1;
}

static int tm_next_less_node(unsigned current)
{
	int min = current;
	int left = tm_left_active_node(current);
	int right = tm_right_active_node(current);

	if (left >= 0 &&
	    tm_cmp_active_node(&tm_active_nodes[left], &tm_active_nodes[min])
	    < 0) {
		min = left;
	}
	if (right >= 0 &&
	    tm_cmp_active_node(&tm_active_nodes[right], &tm_active_nodes[min])
	    < 0) {
		min = right;
	}

	return min == (int) current ? -1 : min;
}

static void tm_descend_active_node(unsigned current)
{
	int	next_node;
	while ((next_node = tm_next_less_node(current)) != -1) {
		tm_active_node_t current_node = tm_active_nodes[current];
		tm_set_active_node(&tm_active_nodes[next_node], current);
		tm_set_active_node(&current_node, next_node);

		current = next_node;
	}
}

static void tm_ascend_active_node(unsigned current)
{
	int parent = tm_parent_active_node(current);
	tm_active_node_t current_node = tm_active_nodes[current];

	while (parent >= 0 &&
	       tm_cmp_active_node(&current_node,
				  &tm_active_nodes[parent]) < 0) {
		tm_set_active_node(&tm_active_nodes[parent], current);

		current = parent;
		parent = tm_parent_active_node(current);
	}
	tm_set_active_node(&current_node, current);
}


static int tm_cmp_active_node(tm_active_node_t *a, tm_active_node_t *b)
{
	if (a->alarm_time.tv_sec < b->alarm_time.tv_sec ||
	    (a->alarm_time.tv_sec == b->alarm_time.tv_sec &&
	     a->alarm_time.tv_nsec < b->alarm_time.tv_nsec)) {
		return -1;
	} else if (a->alarm_time.tv_sec == b->alarm_time.tv_sec &&
		   a->alarm_time.tv_nsec == b->alarm_time.tv_nsec) {
		return 0;
	} else {
		return +1;
	}
}

extern int tm_init()
{
	int		res;
	pthread_attr_t	attr;
	int		i;

	if (tm_active_nodes != NULL) free(tm_active_nodes);
	tm_active_nodes = (tm_active_node_t *) 
		malloc(sizeof(tm_active_node_t) * tm_max_active_nodes);
	if (tm_id2index_map != NULL) free(tm_id2index_map);
	tm_id2index_map = (int *) malloc(sizeof(int) * tm_max_active_nodes);
	for (i = 0; i < tm_max_active_nodes; ++i) {
		tm_id2index_map[i] = TM_INVALID_INDEX;
	}
	tm_current_id = 0;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	tm_main_quit = 0;
	min_node_changed = 0;
	res = pthread_create(&tm_main_thread, &attr, tm_main, NULL);
	pthread_attr_destroy(&attr);

	return res;
}

extern void tm_destroy()
{
	pthread_mutex_lock(&nodes_mutex);
	tm_main_quit = 1;
	if (tm_active_nodes_empty()) {
		pthread_cond_signal(&alarm_cond);
	}
	pthread_mutex_unlock(&nodes_mutex);
	pthread_join(tm_main_thread, NULL);
}

extern tm_timer_t *tm_create_timer(time_t sec, long nsec,
				   tm_handler_t handler)
{
	tm_timer_t *timer = (tm_timer_t *) malloc(sizeof(tm_timer_t));
	if (timer != NULL) {
		timer->interval.tv_sec = sec;
		timer->interval.tv_nsec = nsec;
		timer->handler = handler;
	}

	return timer;
}

extern void tm_destroy_timer(tm_timer_t *timer)
{
	free(timer);
}

/*
 * return: timer identifier that can be used to stop the timer, or < 0.
 */
extern int tm_start_timer(tm_timer_t *timer, void *client_data,
			  unsigned activation)
{
	int	id;

	pthread_mutex_lock(&nodes_mutex);
	id = tm_add_active_node(timer, client_data,
				activation, TM_INVALID_INDEX);
	if (id != TM_INVALID_INDEX && tm_active_nodes[0].id == id) {
		/* the new node is the minimum. */
		min_node_changed = 1;
		pthread_cond_signal(&alarm_cond);
	}
	pthread_mutex_unlock(&nodes_mutex);

	return id;
}

extern void tm_stop_timer(int id)
{
	int	min_stopped;

	pthread_mutex_lock(&nodes_mutex);
	min_stopped = tm_id2index_map[id] == 0;
	tm_remove_active_node(id);
	if (min_stopped) {
		min_node_changed = 1;
		pthread_cond_signal(&alarm_cond);
	}
	pthread_mutex_unlock(&nodes_mutex);
}

static void *tm_main(void *data)
{
	pthread_mutex_lock(&nodes_mutex);
	while (!tm_active_nodes_empty() || !tm_main_quit) {

		min_node_changed = 0;
		if (tm_active_nodes_empty()) {
			pthread_cond_wait(&alarm_cond, &nodes_mutex);
		} else {
			struct timespec *timeout =
				&tm_active_nodes_min()->alarm_time;
			tm_handler_t	handler = NULL;
			void		*client_data;
			int		id;
			int res = pthread_cond_timedwait(&alarm_cond,
							 &nodes_mutex,
							 timeout);
			if (res != EINTR && !min_node_changed) {
				tm_timer_t *timer =
					tm_active_nodes_min()->parent;
				int activation =
					tm_active_nodes_min()->activation;
				handler = timer->handler;
				client_data = 
					tm_active_nodes_min()->client_data;
				id = tm_active_nodes_min()->id;

				tm_remove_active_node(id);
				if (activation == -1 || --activation > 0) {
					tm_add_active_node(timer, client_data,
							   activation, id);
				}
			}

			if (handler != NULL) {
				/* pthread_mutex_unlock(&nodes_mutex); */
				(*handler)(id, client_data);
				/* pthread_mutex_lock(&nodes_mutex); */
			}
		}
	}
	pthread_mutex_unlock(&nodes_mutex);

	return NULL;
}

/* functions for testing */

static void print_time(struct timespec *t)
{
	printf("%2ld sec, %10ld nano sec", t->tv_sec, t->tv_nsec);
}	

static void print_active_node(unsigned i)
{
	printf("parent: %8p\talarm time: ", tm_active_nodes[i].parent);
	print_time(&tm_active_nodes[i].alarm_time);
	putchar('\n');
}

static void print_active_nodes()
{
	int	i;
	for (i = 0; i < tm_active_nodes_length; ++i) {
		print_active_node(i);
	}
}

static void test_active_nodes()
{
	tm_timer_t *timer[6];
	struct timeval	now;
	int		i;

	tm_init();

	gettimeofday(&now, NULL);
	printf("current time: %ld sec, %10ld nano sec\n",
	       now.tv_sec, now.tv_usec * 1000l);

	timer[0] = tm_create_timer(3, 0l, NULL);
	timer[1] = tm_create_timer(5, 0l, NULL);
	timer[2] = tm_create_timer(6, 0l, NULL);
	timer[3] = tm_create_timer(4, 0l, NULL);
	timer[4] = tm_create_timer(1, 0l, NULL);
	timer[5] = tm_create_timer(7, 0l, NULL);

	for (i = 0; i < sizeof(timer)/sizeof(timer[0]); ++i) {
		tm_add_active_node(timer[i], NULL, 1, TM_INVALID_INDEX);
	}
	print_active_nodes();
	
	tm_remove_active_node(tm_active_nodes[0].id);
	putchar('\n');
	print_active_nodes();
}

static void print_handler(int id, char *text)
{
	static int	index = 0;

	printf("handler(%04d, id:%04d): %s\n", ++index, id, text);
}

static void test_tm_main()
{
	tm_timer_t	*timer[2];
	int		ids[2];

	tm_init();

	timer[0] = tm_create_timer(1, 0l, (tm_handler_t) print_handler);
	timer[1] = tm_create_timer(1, 0l, (tm_handler_t) print_handler);

	ids[0] = tm_start_timer(timer[0], "tick!", 4);
	ids[1] = tm_start_timer(timer[1], "tack!", 4);

	usleep(1500000ul);
	printf("main\n");
	tm_stop_timer(ids[0]);

	tm_destroy();
}

static void stress_test_tm_main()
{
	tm_timer_t	*timer[2];
	int		i;

	tm_init();

	timer[0] = tm_create_timer(0, 1000l, (tm_handler_t) print_handler);
	timer[1] = tm_create_timer(0, 1000l, (tm_handler_t) print_handler);

	for (i = 0; i < 300; ++i) {
		tm_start_timer(timer[0], "tick!", 2);
		tm_start_timer(timer[1], "tack!", 1);
	}

	putchar('\n');
	tm_start_timer(timer[0], "tick!", 100);
	tm_start_timer(timer[1], "tack!", 100);

	tm_destroy();
	printf("tm_max_active_nodes: %d\n", tm_max_active_nodes);
}

/*
int main()
{
	test_active_nodes();
	test_tm_main();
	stress_test_tm_main();

	return 0;
}
*/
