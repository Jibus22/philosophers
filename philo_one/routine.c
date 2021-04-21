/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 14:57:02 by jle-corr          #+#    #+#             */
/*   Updated: 2021/04/21 14:57:03 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

static int	take_forks(t_philo *philo, int *living)
{
	pthread_mutex_t	*take_forks;

	take_forks = &(philo->env->mtx_take_forks);
	if (mtx_handler(LOCK, take_forks, living) == FAIL)
		return (DEAD);
	if (*(philo->right_fork) == USED)
	{
		if (mtx_handler(UNLOCK, take_forks, living) == FAIL)
			return (DEAD);
		return (CONTINUE);
	}
	if (mtx_handler(LOCK, philo->mtx_rfork, living) == FAIL)
		return (DEAD);
	*(philo->right_fork) = USED;
	print_new_status(philo, philo->str_id, " has taken a fork\n");
	if (mtx_handler(LOCK, philo->mtx_lfork, living) == FAIL)
		return (DEAD);
	*(philo->left_fork) = USED;
	if (mtx_handler(UNLOCK, take_forks, living) == FAIL)
		return (DEAD);
	print_new_status(philo, philo->str_id, " has taken a fork\n");
	return (EATING);
}

static int	try_to_take_forks(t_philo *philo)
{
	int				ret;
	int				*living;

	living = &(philo->env->living);
	while (*living == ALL_ALIVE)
	{
		if (am_i_dead(philo) == DEAD)
			return (DEAD);
		if (*(philo->right_fork) == AVAILABLE
			&& *(philo->left_fork) == AVAILABLE)
		{
			ret = take_forks(philo, living);
			if (ret == EATING)
				return (EATING);
			else if (ret == DEAD)
				return (DEAD);
			else
				continue ;
		}
		usleep(10);
	}
	return (DEAD);
}

/*
** Dangers : le thread s'arrête en attendant que le mutex soit libre
** donc si un autre thread meurt, comment dire à celui ci d'arrêter d'attendre
** et de quitter ?
*/

static int	wanna_eat(t_philo *philo)
{
	int	*living;

	living = &(philo->env->living);
	philo->state = try_to_take_forks(philo);
	if (philo->state != EATING)
		return (DEAD);
	print_new_status(philo, philo->str_id, " is eating\n");
	philo->last_lunch = get_timestamp(philo->time_start);
	philo->state = sleep_but_listen(philo, philo->env->tte);
	*(philo->right_fork) = AVAILABLE;
	if (mtx_handler(UNLOCK, philo->mtx_rfork, living) == FAIL)
		return (DEAD);
	*(philo->left_fork) = AVAILABLE;
	if (mtx_handler(UNLOCK, philo->mtx_lfork, living) == FAIL)
		return (DEAD);
	if (philo->env->living == DEAD || philo->state == DEAD)
		return (DEAD);
	philo->state = SLEEPING;
	usleep(53);
	return (SLEEPING);
}

static int	go_to_sleep(t_philo *philo)
{
	print_new_status(philo, philo->str_id, " is sleeping\n");
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
		usleep(8000);
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
