#include "philo_one.h"

int			thread_mutex_init(t_env *env, pthread_t **thread,
		pthread_mutex_t **mutex)
{
	pthread_mutex_t	*mutex_adr;
	int				i;

	*thread = (pthread_t *)malloc(sizeof(pthread_t) * env->philo_nb);
	if (!*thread)
		return (FAIL_INIT);
	*mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * env->philo_nb);
	if (!*mutex)
		return (FAIL_INIT);
	i = -1;
	mutex_adr = *mutex;
	while (++i < env->philo_nb)
	{
		if (pthread_mutex_init(&(mutex_adr[i]), NULL))
			return (FAIL_INIT);
	}
	return (0);
}

static void	give_forks(t_philo *philo_cpy, t_env *env,
				pthread_mutex_t *mutex, int i)
{
	if (i == (env->philo_nb - 1))
	{
		philo_cpy->mtx_lfork = &(mutex[0]);
		philo_cpy->left_fork = &(philo_cpy->env->forks[0]);
	}
	else
	{
		philo_cpy->mtx_lfork = &(mutex[i + 1]);
		philo_cpy->left_fork = &(philo_cpy->env->forks[i + 1]);
	}
	philo_cpy->mtx_rfork = &(mutex[i]);
	philo_cpy->right_fork = &(philo_cpy->env->forks[i]);
}

int			philo_init(t_philo *philo, t_env *env, pthread_mutex_t *mutex)
{
	t_philo			*philo_cpy;
	char			*temp_id;
	int				i;

	i = -1;
	while (++i < env->philo_nb)
	{
		philo_cpy = &(philo[i]);
		philo_cpy->env = env;
		philo_cpy->id = i + 1;
		temp_id = ft_itoa(philo_cpy->id);
		if (!temp_id)
			return (FAIL_INIT);
		philo_cpy->str_id[0] = ' ';
		ft_strlcpy(&(philo_cpy->str_id[1]), temp_id, 5);
		free(temp_id);
		philo_cpy->state = THINKING;
		philo_cpy->last_lunch = 0;
		give_forks(philo_cpy, env, mutex, i);
	}
	return (0);
}
