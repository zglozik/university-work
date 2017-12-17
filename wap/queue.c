
#include	"queue.h"

#include	<stdio.h>
#include	<stdlib.h>

/* internal queue representation */

struct queue_t {
	void	**data;
	int	capacity;	/* number of elements allocated for data */
	int	size;		/* number of elements in queue */
	int	next_index;	/* where to put the next element */
	int	last_index;	/* at which index is the last element */
};

/* private function declarations */

static int queue_resize(queue_t *queue);

/* functions definitions */

extern queue_t *queue_create(int initial_size)
{
	queue_t	*queue = (queue_t *) malloc(sizeof(queue_t));
	if (queue == NULL)
		return NULL;
	queue->data = malloc(sizeof(void *) * initial_size);
	if (queue->data == NULL) {
		free(queue);
		return NULL;
	}
	queue->capacity = initial_size;
	queue->size = 0;
	queue->next_index = 0;
	queue->last_index = 0;

	return queue;
}

/*
 * return: -1 means error.
 */
static int queue_resize(queue_t *queue)
{
	int	oldi, newi;
	int	new_capacity = queue->capacity * 1.5;
	void	**new_data = malloc(sizeof(void *) * new_capacity);
	if (new_data == NULL) {
		return -1;
	}

	/* copy data from old array into the new one. */
	for (oldi = queue->last_index, newi = 0; newi < queue->size;
	     oldi = (oldi + 1) % queue->capacity, ++newi) {
		new_data[newi] = queue->data[oldi];
	}

	free(queue->data);
	queue->data = new_data;
	queue->capacity = new_capacity;
	queue->next_index = queue->size;
	queue->last_index = 0;

	return 0;
}

extern void queue_destroy(queue_t *queue)
{
	free(queue->data);
	free(queue);
}

/*
 * return: -1 means error, 0 otherwise.
 */
extern int queue_add(queue_t *queue, void *elem)
{
	if (queue->capacity == queue->size && queue_resize(queue) == -1) {
		return -1;
	}

	queue->data[queue->next_index] = elem;
	queue->next_index = (queue->next_index + 1) % queue->capacity;
	++queue->size;

	return 0;
}

extern void *queue_pop(queue_t *queue)
{
	void	*result;

	if (queue->size == 0) {
		return NULL;
	}
	result = queue->data[queue->last_index];
	queue->last_index = (queue->last_index + 1) % queue->capacity;
	--queue->size;

	return result;
}

extern void *queue_top(queue_t *queue)
{
	if (queue->size == 0) {
		return NULL;
	}

	return queue->data[queue->last_index];
}

extern int queue_size(queue_t *queue)
{
	return queue->size;
}

extern int queue_empty(queue_t *queue)
{
	return queue->size == 0;
}

/* test main function */

static void functional_test()
{
	int	elem;
	queue_t	*queue = queue_create(2);

	queue_add(queue, (void *) 3);
	queue_add(queue, (void *) 4);
	queue_pop(queue);
	queue_pop(queue);
	queue_add(queue, (void *) 5);
	queue_add(queue, (void *) 6);
	queue_add(queue, (void *) 7);
	queue_add(queue, (void *) 8);
	queue_add(queue, (void *) 9);
	queue_add(queue, (void *) 10);

	printf("queue capacity: %d\n", queue->capacity);

	printf("top elem: %d\n", (int) queue_top(queue));
	while ((elem = (int) queue_pop(queue)) != 0) {
		printf("elem: %d\n", elem);
	}
	queue_destroy(queue);
}

static void regression_test()
{
	int	i, elem;
	queue_t	*queue = queue_create(10);

	for (i = 1; i <= 500; ++i) {
		queue_add(queue, (void *) i);
	}
	for (i = 1; i <= 200; ++i) {
		queue_pop(queue);
	}
	for (i = 1; i <= 200; ++i) {
		queue_add(queue, (void *) i);
	}

	printf("queue capacity: %d\n", queue->capacity);
	printf("queue size: %d\n", queue_size(queue));

	while ((elem = (int) queue_pop(queue)) != 0) {
		printf("elem: %d\n", elem);
	}
	queue_destroy(queue);

}

/*
int main()
{

	functional_test()
	regression_test();

	return 0;
}
*/
