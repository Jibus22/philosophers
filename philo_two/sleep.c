/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 14:57:08 by jle-corr          #+#    #+#             */
/*   Updated: 2021/04/21 14:57:08 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int			micro_sleep(t_philo *philo, long end)
{
	long	timestamp;

	timestamp = get_timestamp(philo->time_start);
	while (timestamp < end)
	{
		usleep(500);
		timestamp = get_timestamp(philo->time_start);
	}
	return (0);
}

int			sleep_but_listen(t_philo *philo, int duration)
{
	long	timestamp;
	long	time_end;
	int		first_part;

	timestamp = get_timestamp(philo->time_start);
	time_end = timestamp + (long)duration;
	if ((philo->last_lunch + (long)philo->env->ttd) < time_end)
	{
		time_end = philo->last_lunch + (long)philo->env->ttd;
		duration = (int)time_end - (int)timestamp;
	}
	first_part = duration * 90 / 100;
	usleep(first_part * 1000);
	micro_sleep(philo, time_end);
	if (am_i_dead(philo) == DEAD || philo->env->living == DEAD)
		return (DEAD);
	return (philo->state);
}
