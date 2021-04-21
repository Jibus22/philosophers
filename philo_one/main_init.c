#include "philo_one.h"

void		set_struct_null(t_philo **philo, t_env **env, pthread_t **thread,
		pthread_mutex_t **mutex)
{
	*philo = NULL;
	*env = NULL;
	*thread = NULL;
	*mutex = NULL;
}

static void	init_forks(char *forks, int nb)
{
	int		i;

	i = -1;
	while (++i < nb)
		forks[i] = AVAILABLE;
}

static int	init_arguments(t_env *env, char *av[], int arg_nb)
{
	int		i;
	int		*envir;
	char	*endptr;

	i = 0;
	errno = 0;
	envir = &(env->philo_nb);
	while (++i <= arg_nb)
	{
		*envir = ft_strtoimax(av[i], &endptr);
		if (*endptr || errno == ERANGE)
			return (-1);
		envir += (int)1;
	}
	return (0);
}

static int	init_env(int ac, char *av[], t_env *env)
{
	if (ac < 5 || ac > 6)
		return (FAIL_INIT);
	if (init_arguments(env, av, (ac - 1)) == -1)
		return (FAIL_INIT);
	if (ac == 6 && env->max_meal < 1)
		return (FAIL_INIT);
	env->living = ALL_ALIVE;
	if (env->philo_nb == 0)
		return (FAIL_INIT);
	env->forks = (char *)malloc(sizeof(char) * env->philo_nb);
	if (env->forks == NULL)
		return (FAIL_INIT);
	init_forks(env->forks, env->philo_nb);
	if (pthread_mutex_init(&(env->mtx_output), NULL))
		return (FAIL_INIT);
	if (pthread_mutex_init(&(env->mtx_take_forks), NULL))
		return (FAIL_INIT);
	return (0);
}

int			main_init(t_philo **philo, t_env **env, int ac, char *av[])
{
	*env = (t_env *)malloc(sizeof(t_env) * 1);
	if (!*env)
		return (FAIL_INIT);
	memset(*env, 0, sizeof(**env));
	if (init_env(ac, av, *env) == -1)
		return (FAIL_INIT);
	*philo = (t_philo *)malloc(sizeof(t_philo) * (*env)->philo_nb);
	if (!*philo)
		return (FAIL_INIT);
	memset(*philo, 0, sizeof(**philo));
	return (0);
}
