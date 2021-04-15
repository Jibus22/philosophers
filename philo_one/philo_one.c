#include "philo_one.h"

int	get_philo_id(t_philo *philo)
{
	int	i;

	i = 0;
	while (++i < (philo->env->philo_nb + 1))
		if (philo->thread[i] == philo->current_thread_id)
			return (i);
	return (-1);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	int		philo_id;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&(philo->mutex[0]));
	philo_id = get_philo_id(philo);
	printf("curr_th_id: %lu\t\t\tph_id: %3d\n",
			(unsigned long)philo->current_thread_id, philo_id);
	fflush(stdout);
	pthread_mutex_unlock(&(philo->mutex[0]));
	return (NULL);
}

int	launch_thread(t_philo *philo)
{
	int			i;

	i = 0;
	while (++i < (philo->env->philo_nb + 1))
	{
		pthread_mutex_lock(&(philo->mutex[0]));
		pthread_create(&(philo->thread[i]), NULL, routine, philo);
		philo->current_thread_id = philo->thread[i];
		pthread_mutex_unlock(&(philo->mutex[0]));
	}
	return (0);
}

int	exit_thread(t_env *env, pthread_t *thread)
{
	int	i;

	i = 0;
	while (++i < (env->philo_nb + 1))
		pthread_join(thread[i], NULL);
	return (0);
}

int	main(int ac, char *av[])
{
	t_env			*env;
	t_philo			*philo;
	pthread_t		*thread;
	pthread_mutex_t	*mutex;

	if (main_init(&philo, &env, &thread, &mutex) == -1)
		return (main_exit(philo, env, thread, mutex));
	if (init_env(ac, av, env) == -1)
		return (main_exit(philo, env, thread, mutex));
	print_env(env);
	if (thread_mutex_init(env, &thread, &mutex) == -1)
		return (main_exit(philo, env, thread, mutex));
	philo_init(philo, env, thread, mutex);
	launch_thread(philo);
	print_thread(env, thread);
	exit_thread(env, thread);
	free(thread);
	//system("leaks philo_one");
	return (0);
}
