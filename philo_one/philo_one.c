#include "philo_one.h"

void	get_timestamp(struct timeval tv_start, long *time_stamp)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	printf("id: %d\n", philo->id);
	printf("sec: %ld\tusec: %ld\n", (long)(philo->time_start.tv_sec),
			(long)(philo->time_start.tv_usec));
	fflush(stdout);
	get_timestamp(philo->time_start, &(philo->timestamp));
	return (NULL);
}

int	launch_thread(t_env *env, pthread_t *thread, t_philo *philo)
{
	struct timeval	tv;
	int				i;

	gettimeofday(&tv, NULL);
	i = -1;
	while (++i < env->philo_nb)
	{
		philo[i].time_start = tv;
		pthread_create(&(thread[i]), NULL, routine, &(philo[i]));
	}
	return (0);
}

int	exit_thread(t_env *env, pthread_t *thread)
{
	int	i;

	i = -1;
	while (++i < env->philo_nb)
		pthread_join(thread[i], NULL);
	return (0);
}

int	main(int ac, char *av[])
{
	t_env			*env;
	t_philo			*philo;
	pthread_t		*thread;
	pthread_mutex_t	*mutex;

	set_struct_null(&philo, &env, &thread, &mutex);
	if (main_init(&philo, &env, ac, av) == -1)
		return (main_exit(philo, env, thread, mutex));
	if (thread_mutex_init(env, &thread, &mutex) == -1)
		return (main_exit(philo, env, thread, mutex));
	philo_init(philo, env, mutex);
	launch_thread(env, thread, philo);
	exit_thread(env, thread);
	free(thread);
	return (0);
}

/*

	//print_env(env);
	//print_thread(env, thread);
	//system("leaks philo_one");


*/
