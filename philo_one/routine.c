#include "philo_one.h"

int		try_to_take_forks(t_philo *philo)
{
	while (philo->env->living == ALL_ALIVE)
	{
		if (am_i_dead(philo) == DEAD)
			return (DEAD);
		if (*(philo->right_fork) == AVAILABLE
			&& *(philo->left_fork) == AVAILABLE)
		{
			if (*(philo->right_fork) == USED)
				continue ;
			pthread_mutex_lock(philo->mtx_rfork);
			*(philo->right_fork) = USED;
			print_new_status(philo, philo->str_id, TAKE_FORK,
				philo->status_buf);
			pthread_mutex_lock(philo->mtx_lfork);
			*(philo->left_fork) = USED;
			print_new_status(philo, philo->str_id, TAKE_FORK,
				philo->status_buf);
			return (EATING);
		}
		usleep(1000);
	}
	return (DEAD);
}

/*
** Dangers : le thread s'arrête en attendant que le mutex soit libre
** donc si un autre thread meurt, comment dire à celui ci d'arrêter d'attendre
** et de quitter ?
*/

int		wanna_eat(t_philo *philo)
{
	philo->state = try_to_take_forks(philo);
	if (philo->state != EATING)
		return (DEAD);
	print_new_status(philo, philo->str_id, EATING, philo->status_buf);
	philo->last_lunch = get_timestamp(philo->time_start);
	philo->state = sleep_but_listen(philo, philo->env->tte);
	*(philo->right_fork) = AVAILABLE;
	pthread_mutex_unlock(philo->mtx_rfork);
	*(philo->left_fork) = AVAILABLE;
	pthread_mutex_unlock(philo->mtx_lfork);
	if (philo->env->living == DEAD || philo->state == DEAD)
		return (DEAD);
	philo->state = SLEEPING;
	return (philo->state);
}

int		go_to_sleep(t_philo *philo)
{
	print_new_status(philo, philo->str_id, SLEEPING, philo->status_buf);
	philo->state = sleep_but_listen(philo, philo->env->tts);
	if (philo->env->living == DEAD || philo->state == DEAD)
		return (DEAD);
	philo->state = THINKING;
	print_new_status(philo, philo->str_id, THINKING, philo->status_buf);
	return (philo->state);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	print_new_status(philo, philo->str_id, THINKING, philo->status_buf);
	philo->last_lunch = get_timestamp(philo->time_start);
	if (philo->id % 2 == 0)
		usleep(2000);
	while (philo->env->living == ALL_ALIVE && philo->state != DEAD)
	{
		if (wanna_eat(philo) == DEAD)
			break ;
		if (go_to_sleep(philo) == DEAD)
			break ;
	}
	return (NULL);
}
