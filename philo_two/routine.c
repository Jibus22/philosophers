/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 14:57:02 by jle-corr          #+#    #+#             */
/*   Updated: 2021/04/22 02:34:24 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

static int	take_forks(t_philo *philo, int *fork_lock)
{
	sem_wait(philo->env->sem_handle_forks);
	*fork_lock = LOCK;
	sem_wait(philo->sem_forks);
	print_new_status(philo, philo->str_id, " has taken a fork\n");
	sem_wait(philo->sem_forks);
	print_new_status(philo, philo->str_id, " has taken a fork\n");
	sem_post(philo->env->sem_handle_forks);
	*fork_lock = UNLOCK;
	return (EATING);
}

static int	try_to_take_forks(t_philo *philo)
{
	int		ret;
	int		*fork_lock;
	int		*living;

	living = &(philo->env->living);
	fork_lock = &(philo->env->fork_lock);
	while (*living == ALL_ALIVE)
	{
		if (am_i_dead(philo) == DEAD)
			return (DEAD);
		if (*fork_lock == UNLOCK)
		{
			ret = take_forks(philo, fork_lock);
			if (ret == EATING)
				return (EATING);
			else if (ret == DEAD)
				return (DEAD);
		}
		usleep(10);
	}
	return (DEAD);
}

static int	wanna_eat(t_philo *philo)
{
	int		*living;

	living = &(philo->env->living);
	philo->state = try_to_take_forks(philo);
	if (philo->state != EATING)
		return (DEAD);
	print_new_status(philo, philo->str_id, " is eating\n");
	philo->last_lunch = get_timestamp(philo->time_start);
	philo->state = sleep_but_listen(philo, philo->env->tte);
	sem_post(philo->sem_forks);
	sem_post(philo->sem_forks);
	if (philo->env->living == DEAD || philo->state == DEAD)
		return (DEAD);
	philo->state = SLEEPING;
	return (SLEEPING);
}

static int	go_to_sleep(t_philo *philo)
{
	print_new_status(philo, philo->str_id, " is sleeping\n");
	usleep(53);
	philo->state = sleep_but_listen(philo, philo->env->tts);
	if (philo->env->living == DEAD || philo->state == DEAD)
		return (DEAD);
	philo->state = THINKING;
	print_new_status(philo, philo->str_id, " is thinking\n");
	usleep(379);
	return (THINKING);
}

void		*routine(void *arg)
{
	t_philo	*philo;
	int		*meal_nb;
	int		*max_meal;

	philo = (t_philo *)arg;
	meal_nb = &(philo->meal_nb);
	max_meal = &(philo->env->max_meal);
	if (philo->id % 2 == 0)
		usleep(500 * philo->env->tte);
	print_new_status(philo, philo->str_id, " is thinking\n");
	philo->last_lunch = get_timestamp(philo->time_start);
	while (philo->env->living == ALL_ALIVE && philo->state != DEAD)
	{
		if (wanna_eat(philo) == DEAD)
			break ;
		*meal_nb += 1;
		if (*meal_nb == *max_meal)
			break ;
		if (go_to_sleep(philo) == DEAD)
			break ;
	}
	return (NULL);
}
