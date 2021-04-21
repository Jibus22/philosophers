#include "philo_one.h"

int			destroy_exit(t_philo *philo, t_env *env,
		pthread_t *thread, pthread_mutex_t *mutex)
{
	if (philo)
		free(philo);
	if (thread)
		free(thread);
	if (mutex)
		free(mutex);
	if (env)
	{
		if (env->forks)
			free(env->forks);
		free(env);
	}
	return (1);
}
