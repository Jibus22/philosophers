#include "philo_one.h"

static int	data_struct_create(t_env **env, t_philo **philo)
{
	*env = (t_env *)malloc(sizeof(t_env) * 1);
	if (!*env)
		return (-1);
	*philo = (t_philo *)malloc(sizeof(t_philo) * 1);
	if (!*philo)
		return (-1);
	return (0);
}

int			main_init(t_philo **philo, t_env **env,
		pthread_t **thread, pthread_mutex_t **mutex)
{
	*philo = NULL;
	*env = NULL;
	*thread = NULL;
	*mutex = NULL;
	if (data_struct_create(env, philo) == -1)
		return (-1);
	return (0);
}

int			thread_mutex_init(t_env *env, pthread_t **thread,
		pthread_mutex_t **mutex)
{
	pthread_mutex_t	*mutex_adr;
	int				unit_nb;
	int				i;

	unit_nb = env->philo_nb + 1;
	*thread = (pthread_t *)malloc(sizeof(pthread_t) * unit_nb);
	if (!*thread)
		return (-1);
	*mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * unit_nb);
	if (!*mutex)
		return (-1);
	i = -1;
	mutex_adr = *mutex;
	while (++i < unit_nb)
		pthread_mutex_init(&(mutex_adr[i]), NULL);
	return (0);
}

void		philo_init(t_philo *philo, t_env *env,
		pthread_t *thread, pthread_mutex_t *mutex)
{
	philo->env = env;
	philo->thread = thread;
	philo->mutex = mutex;
}

int			init_env(int ac, char *av[], t_env *env)
{
	int		i;
	int		*envir;
	char	*endptr;

	if (ac != 5)
		return (-1);
	i = 0;
	errno = 0;
	envir = &(env->philo_nb);
	while (++i <= 4)
	{
		*envir = ft_strtoimax(av[i], &endptr);
		if (*endptr || errno == ERANGE)
			return (-1);
		envir += (int)1;
	}
	if (env->philo_nb == 0)
		return (-1);
	*envir = env->philo_nb;
	return (0);
}
