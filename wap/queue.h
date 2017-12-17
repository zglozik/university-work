
#ifndef	_QUEUE_H_
#define	_QUEUE_H_

/* Queue container implementation interface file. */

typedef struct queue_t	queue_t;

extern queue_t	*queue_create(int initial_size);
extern void	queue_destroy(queue_t *queue);

extern int	queue_add(queue_t *queue, void *elem);
extern void	*queue_pop(queue_t *queue);
extern void	*queue_top(queue_t *queue);
extern int	queue_size(queue_t *queue);
extern int	queue_empty(queue_t *queue);

#endif
