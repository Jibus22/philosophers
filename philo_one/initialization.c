#include "philo_one.h"

static int	init_env(int ac, char *av[], t_env *env)
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
	return (0);
}

int			main_init(t_philo **philo, t_env **env, int ac, char *av[])
{
	*env = (t_env *)malloc(sizeof(t_env) * 1);
	if (!*env)
		return (-1);
	if (init_env(ac, av, *env) == -1)
		return (-1);
	*philo = (t_philo *)malloc(sizeof(t_philo) * (*env)->philo_nb);
	if (!*philo)
		return (-1);
	return (0);
}

int			thread_mutex_init(t_env *env, pthread_t **thread,
		pthread_mutex_t **mutex)
{
	pthread_mutex_t	*mutex_adr;
	int				i;

	*thread = (pthread_t *)malloc(sizeof(pthread_t) * env->philo_nb);
	if (!*thread)
		return (-1);
	*mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * env->philo_nb);
	if (!*mutex)
		return (-1);
	i = -1;
	mutex_adr = *mutex;
	while (++i < env->philo_nb)
		pthread_mutex_init(&(mutex_adr[i]), NULL);
	return (0);
}

void		philo_init(t_philo *philo, t_env *env, pthread_mutex_t *mutex)
{
	t_philo			*philo_cpy;
	int				i;

	i = -1;
	while (++i < env->philo_nb)
	{
		philo_cpy = &(philo[i]);
		philo_cpy->env = env;
		philo_cpy->id = i + 1;
		if (i == (env->philo_nb - 1))
			philo_cpy->left_fork = &(mutex[0]);
		else
			philo_cpy->left_fork = &(mutex[i + 1]);
		philo_cpy->right_fork = &(mutex[i]);
	}
}

void		set_struct_null(t_philo **philo, t_env **env, pthread_t **thread,
		pthread_mutex_t **mutex)
{
	*philo = NULL;
	*env = NULL;
	*thread = NULL;
	*mutex = NULL;
}
