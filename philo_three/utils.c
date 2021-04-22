/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 14:57:21 by jle-corr          #+#    #+#             */
/*   Updated: 2021/04/22 14:43:10 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

long	get_timestamp(struct timeval tv_start)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (((now.tv_sec - tv_start.tv_sec) * 1000)
	+ ((now.tv_usec - tv_start.tv_usec) / 1000));
}

void	set_buffer(char *buf, char *timestamp, char *id, char *status)
{
	ft_strlcpy(buf, timestamp, STAT_BUF_LEN);
	ft_strlcat(buf, id, STAT_BUF_LEN);
	ft_strlcat(buf, status, STAT_BUF_LEN);
}

int		print_new_status(t_philo *philo, char *id, char *status)
{
	sem_t	*sem_output;
	int		*living;
	char	*tmp_str_timestamp;
	char	buf[STAT_BUF_LEN];
	long	timestamp;

	memset(buf, 0, STAT_BUF_LEN);
	sem_output = philo->env->sem_output;
	living = &(philo->env->living);
	tmp_str_timestamp = NULL;
	sem_wait(sem_output);
	timestamp = get_timestamp(philo->time_start);
	tmp_str_timestamp = ft_itoa((int)timestamp);
	if (!tmp_str_timestamp)
		return (FAIL);
	set_buffer(buf, tmp_str_timestamp, id, status);
	free(tmp_str_timestamp);
	if (*living == DEAD && philo->state != DIED)
	{
		sem_post(sem_output);
		return (FAIL);
	}
	write(1, buf, STAT_BUF_LEN);
	sem_post(sem_output);
	return (SUCCESS);
}

int		am_i_dead(t_philo *philo)
{
	long	timestamp;

	timestamp = get_timestamp(philo->time_start);
	if ((timestamp - philo->last_lunch) > (long)philo->env->ttd)
	{
		if (philo->env->living == ALL_ALIVE)
		{
			philo->env->living = DEAD;
			philo->state = DIED;
		}
		else
			return (DEAD);
		print_new_status(philo, philo->str_id, " has died\n");
		return (DEAD);
	}
	else
		return (NOT_YET);
}

int		mtx_handler(int code, pthread_mutex_t *mtx, int *living)
{
	int	ret;

	ret = SUCCESS;
	if (code == LOCK)
		if (pthread_mutex_lock(mtx))
			ret = FAIL;
	if (code == UNLOCK)
		if (pthread_mutex_unlock(mtx))
			ret = FAIL;
	if (ret == FAIL)
		*living = DEAD;
	return (ret);
}
