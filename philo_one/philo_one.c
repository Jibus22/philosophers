#include "philo_one.h"

int	launch_thread(t_env *env, pthread_t *thread, t_philo *philo)
{
	struct timeval	start_time_value;
	int				i;

	gettimeofday(&start_time_value, NULL);
	i = -1;
	while (++i < env->philo_nb)
	{
		philo[i].time_start = start_time_value;
		if (pthread_create(&(thread[i]), NULL, routine, &(philo[i])))
		{
			env->living = DEAD;
			return (-1);
		}
	}
	return (0);
}

int	exit_thread(t_env *env, pthread_t *thread, pthread_mutex_t *mutex)
{
	int	i;

	i = -1;
	while (++i < env->philo_nb)
		pthread_join(thread[i], NULL);
	i = -1;
	while (++i < env->philo_nb)
	{
		if (pthread_mutex_destroy(&(mutex[i])))
		{
			printf("wwooops\n");
			return (FAIL_INIT);
		}
	}
	if (pthread_mutex_destroy(&(env->mtx_take_forks)))
	{
		printf("wwooops_mtxtakeforks\n");
		return (FAIL_INIT);
	}
	if (pthread_mutex_destroy(&(env->mtx_output)))
	{
		printf("wwooops_mtxoutput\n");
		return (FAIL_INIT);
	}
	return (0);
}

int	main(int ac, char *av[])
{
	t_env			*env;
	t_philo			*philo;
	pthread_t		*thread;
	pthread_mutex_t	*mutex;

	set_struct_null(&philo, &env, &thread, &mutex);
	if (main_init(&philo, &env, ac, av) == FAIL_INIT)
		return (main_exit(philo, env, thread, mutex));
	if (thread_mutex_init(env, &thread, &mutex) == FAIL_INIT)
		return (main_exit(philo, env, thread, mutex));
	if (philo_init(philo, env, mutex) == FAIL_INIT)
		return (main_exit(philo, env, thread, mutex));
	launch_thread(env, thread, philo);
	exit_thread(env, thread, mutex);
	free(thread);
	return (0);
}

/*

	print_env(env);
	print_thread(env, thread);
	system("leaks philo_one");
*/
