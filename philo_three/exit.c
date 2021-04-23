/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 14:54:51 by jle-corr          #+#    #+#             */
/*   Updated: 2021/04/23 11:32:51 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int			destroy_exit(t_philo *philo, t_env *env)
{
	sem_unlink(env->sem_forks_name);
	sem_unlink(env->sem_handle_forks_name);
	sem_unlink(env->sem_output_name);
	sem_close(env->sem_forks);
	sem_close(env->sem_handle_forks);
	sem_close(env->sem_output);
	if (philo)
		free(philo);
	if (env)
	{
		if (env->c_pid)
			free(env->c_pid);
		free(env);
	}
	return (1);
}
