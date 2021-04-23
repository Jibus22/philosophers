/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 14:56:49 by jle-corr          #+#    #+#             */
/*   Updated: 2021/04/23 12:06:43 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int	kill_all_childs(t_env *env)
{
	int	i;

	i = -1;
	while (++i < env->philo_nb)
	{
		if (env->c_pid[i] == -1)
			return (0);
		kill(env->c_pid[i], SIGTERM);
	}
	return (0);
}

int	wait_childs(t_env *env)
{
	int	status;
	int	child_ret;
	int	ret;

	ret = 0;
	while ((ret = waitpid(0, &status, 0)) > -1)
	{
		if (WIFEXITED(status))
			child_ret = WEXITSTATUS(status);
		if (child_ret == DEAD)
		{
			kill_all_childs(env);
			return (1);
		}
		else
			continue;
	}
	return (0);
}

int	launch_childs(t_env *env, t_philo *philo)
{
	struct timeval	start_time_value;
	int				*c_pid;
	int				i;

	gettimeofday(&start_time_value, NULL);
	c_pid = env->c_pid;
	i = -1;
	while (++i < env->philo_nb)
	{
		philo[i].time_start = start_time_value;
		c_pid[i] = fork();
		if (c_pid[i] == -1)
		{
			kill_all_childs(env);
			return (0);
		}
		else if (c_pid[i] == 0)
		{
			routine(&(philo[i]));
			return (0);
		}
	}
	wait_childs(env);
	return (0);
}

int	main(int ac, char *av[])
{
	t_env			*env;
	t_philo			*philo;

	set_struct_null(&philo, &env);
	if (main_init(&philo, &env, ac, av) == FAIL_INIT)
		return (destroy_exit(philo, env));
	if (philo_init(philo, env) == FAIL_INIT)
		return (destroy_exit(philo, env));
	launch_childs(env, philo);
	destroy_exit(philo, env);
	return (0);
}
