#include "philo_one.h"

long	get_timestamp(struct timeval tv_start)
{
	struct timeval	now;
	long			seconds;
	long			timestamp;

	gettimeofday(&now, NULL);
	seconds = (long)(now.tv_sec) - (long)(tv_start.tv_sec);
	timestamp = (1000000 * seconds) + (long)(now.tv_usec)
		- (long)(tv_start.tv_usec);
	timestamp /= 1000;
	return (timestamp);
}

void	print_new_status(t_philo *philo, char *id, int msg_code, char *buf)
{
	char	*tmp_str_timestamp;
	long	timestamp;

	memset(buf, 0, STAT_BUF_LEN);
	pthread_mutex_lock(&(philo->env->mtx_output));
	timestamp = get_timestamp(philo->time_start);
	tmp_str_timestamp = ft_itoa((int)timestamp);
	ft_strlcpy(buf, tmp_str_timestamp, STAT_BUF_LEN);
	free(tmp_str_timestamp);
	ft_strlcat(buf, id, STAT_BUF_LEN);
	if (msg_code == TAKE_FORK)
		ft_strlcat(buf, " has taken a fork\n", STAT_BUF_LEN);
	else if (msg_code == EATING)
		ft_strlcat(buf, " is eating\n", STAT_BUF_LEN);
	else if (msg_code == SLEEPING)
		ft_strlcat(buf, " is sleeping\n", STAT_BUF_LEN);
	else if (msg_code == DIED)
		ft_strlcat(buf, " has died\n", STAT_BUF_LEN);
	else if (msg_code == THINKING)
		ft_strlcat(buf, " is thinking\n", STAT_BUF_LEN);
	if (philo->env->living == DEAD && philo->state != DIED)
	{
		pthread_mutex_unlock(&(philo->env->mtx_output));
		return ;
	}
	write(1, buf, STAT_BUF_LEN);
	pthread_mutex_unlock(&(philo->env->mtx_output));
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
		print_new_status(philo, philo->str_id, DIED, philo->status_buf);
		return (DEAD);
	}
	else
		return (NOT_YET);
}
