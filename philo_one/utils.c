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
		return ;
	write(1, buf, STAT_BUF_LEN);
}

int		am_i_dead(t_philo *philo, long time_stamp)
{
	long	fresh_time_stamp;

	if (time_stamp < 0)
	{
		fresh_time_stamp = get_timestamp(philo->time_start);
		philo->timestamp = fresh_time_stamp;
	}
	else
		fresh_time_stamp = time_stamp;
	if ((fresh_time_stamp - philo->last_lunch) > (long)philo->env->ttd)
	{
		if (philo->env->living == ALL_ALIVE)
		{
			philo->env->living = DEAD;
			philo->state = DIED;
		}
		else
			return (YES_IM_SORRY);
		print_new_status(philo, philo->str_id, DIED, philo->status_buf);
		return (YES_IM_SORRY);
	}
	else
		return (NOT_YET);
}
