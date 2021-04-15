#include "philo_one.h"

int			main_exit(t_philo *philo, t_env *env,
		pthread_t *thread, pthread_mutex_t *mutex)
{
	if (philo)
		free(philo);
	if (env)
		free(env);
	if (thread)
		free(thread);
	if (mutex)
		free(mutex);
	return (1);
}
