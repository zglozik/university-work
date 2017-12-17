
#ifndef _TIMER_MANAGER_H_
#define	_TIMER_MANAGER_H_

#include	<sys/time.h>

/* public declarations */

typedef void	(*tm_handler_t)(int id, void *data);

typedef struct tm_timer_t	tm_timer_t;

extern int		tm_init();
extern void		tm_destroy();
extern tm_timer_t	*tm_create_timer(time_t sec, long nsec,
					 tm_handler_t handler);
extern void		tm_destroy_timer(tm_timer_t *timer);

/* activation means how many times we should generate alarm,
   -1 means infinite */
extern int	tm_start_timer(tm_timer_t *timer, void *client_data,
			       unsigned activation);

extern void	tm_stop_timer(int id);

#endif
