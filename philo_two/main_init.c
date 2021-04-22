/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 14:56:38 by jle-corr          #+#    #+#             */
/*   Updated: 2021/04/22 03:06:28 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

void		set_struct_null(t_philo **philo, t_env **env, pthread_t **thread)
{
	*philo = NULL;
	*env = NULL;
	*thread = NULL;
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

int			init_semaphores(t_env *env)
{
	ft_strlcpy(env->sem_forks_name, "sem_forks_dude", 15);
	ft_strlcpy(env->sem_handle_forks_name, "sem_handle_forks_folk", 22);
	ft_strlcpy(env->sem_output_name, "sem_output_boy", 15);
	sem_unlink(env->sem_forks_name);
	sem_unlink(env->sem_handle_forks_name);
	sem_unlink(env->sem_output_name);
	env->sem_forks = sem_open(env->sem_forks_name, O_CREAT, 0700,
			env->philo_nb);
	if (env->sem_forks == SEM_FAILED)
		return (FAIL_INIT);
	env->sem_handle_forks = sem_open(env->sem_handle_forks_name,
			O_CREAT, 0700, 1);
	if (env->sem_handle_forks == SEM_FAILED)
		return (FAIL_INIT);
	env->sem_output = sem_open(env->sem_output_name, O_CREAT, 0700, 1);
	if (env->sem_output == SEM_FAILED)
		return (FAIL_INIT);
	return (SUCCESS);
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
	if (init_semaphores(env) == FAIL_INIT)
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
	memset(*philo, 0, (sizeof(**philo) * (*env)->philo_nb));
	return (0);
}
