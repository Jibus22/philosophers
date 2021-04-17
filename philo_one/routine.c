#include "philo_one.h"

void	unlock_n_return(t_philo *philo, char c)
{
	if (c == 'r')
		pthread_mutex_unlock(philo->right_fork);
	else if (c == 'l')
		pthread_mutex_unlock(philo->left_fork);
	else if (c == 'b')
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	return ;
}

/*
** Dangers : le thread s'arrête en attendant que le mutex soit libre
** donc si un autre thread meurt, comment dire à celui ci d'arrêter d'attendre
** et de quitter ?
*/

void	wanna_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	if (am_i_dead(philo, -1) == YES_IM_SORRY || philo->env->living == DEAD)
		return (unlock_n_return(philo, 'r'));
	print_new_status(philo, philo->str_id, TAKE_FORK, philo->status_buf);
	pthread_mutex_lock(philo->left_fork);
	if (am_i_dead(philo, -1) == YES_IM_SORRY || philo->env->living == DEAD)
		return (unlock_n_return(philo, 'b'));
	print_new_status(philo, philo->str_id, TAKE_FORK, philo->status_buf);
	philo->state = EATING;
	print_new_status(philo, philo->str_id, EATING, philo->status_buf);
	philo->last_lunch = get_timestamp(philo->time_start);
	if (sleep_but_listen(philo, philo->env->tte) == DIED
			|| philo->env->living == DEAD)
		return (unlock_n_return(philo, 'b'));
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	philo->state = SLEEPING;
}

void	going_to_sleep(t_philo *philo)
{
	if (sleep_but_listen(philo, philo->env->tts) == DIED
			|| philo->env->living == DEAD)
		return ;
	philo->state = THINKING;
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (philo->env->living == ALL_ALIVE)
	{
		if (philo->state == THINKING)
		{
			print_new_status(philo, philo->str_id, THINKING, philo->status_buf);
			wanna_eat(philo);
		}
		else if (philo->state == SLEEPING)
		{
			print_new_status(philo, philo->str_id, SLEEPING, philo->status_buf);
			going_to_sleep(philo);
		}
	}
	return (NULL);
}
