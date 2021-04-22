/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 14:56:49 by jle-corr          #+#    #+#             */
/*   Updated: 2021/04/23 00:23:36 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

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

int	exit_thread(t_env *env, pthread_t *thread)
{
	int	i;

	i = -1;
	while (++i < env->philo_nb)
		if (pthread_join(thread[i], NULL))
			return (FAIL_DESTROY);
	sem_unlink(env->sem_forks_name);
	sem_unlink(env->sem_handle_forks_name);
	sem_unlink(env->sem_output_name);
	sem_close(env->sem_forks);
	sem_close(env->sem_handle_forks);
	sem_close(env->sem_output);
	return (0);
}

int	main(int ac, char *av[])
{
	t_env			*env;
	t_philo			*philo;
	pthread_t		*thread;

	set_struct_null(&philo, &env, &thread);
	if (main_init(&philo, &env, ac, av) == FAIL_INIT)
		return (destroy_exit(philo, env, thread));
	if (thread_init(env, &thread) == FAIL_INIT)
		return (destroy_exit(philo, env, thread));
	if (philo_init(philo, env) == FAIL_INIT)
		return (destroy_exit(philo, env, thread));
	launch_thread(env, thread, philo);
	exit_thread(env, thread);
	destroy_exit(philo, env, thread);
	return (0);
}

/*

	print_env(env);
	print_thread(env, thread);
	system("leaks philo_one");
*/
