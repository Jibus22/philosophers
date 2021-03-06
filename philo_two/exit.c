/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 14:54:51 by jle-corr          #+#    #+#             */
/*   Updated: 2021/04/22 01:03:00 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

int			destroy_exit(t_philo *philo, t_env *env, pthread_t *thread)
{
	if (philo)
		free(philo);
	if (thread)
		free(thread);
	if (env)
		free(env);
	return (1);
}
