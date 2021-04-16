#include "philo_one.h"

long	get_timestamp(struct timeval tv_start)
{
	struct timeval	now;
	long			seconds;
	long			u_seconds;
	long			time;

	gettimeofday(&now, NULL);
	seconds = (long)(now.tv_sec) - (long)(tv_start.tv_sec);
	u_seconds = (long)(now.tv_usec);
	time = (1000000 * seconds) + u_seconds;
	time = time - (long)(tv_start.tv_usec);
	time /= 1000;
	return (time);
}

void	print_new_status(long timestamp, char *id, int msg_code, char *buf)
{
	char	*tmp_str_timestamp;

	memset(buf, 0, STAT_BUF_LEN);
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
	write(1, buf, STAT_BUF_LEN);
	fflush(stdout);
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
	if ((fresh_time_stamp - philo->last_lunch) > philo->env->ttd)
	{
		philo->state = DIED;
		print_new_status(philo->timestamp, philo->str_id, DEAD,
				philo->status_buf);
		philo->env->living = DEAD;
		return (YES_IM_SORRY);
	}
	else
		return (NOT_YET);
}

int	sleep_but_listen(t_philo *philo, int duration)
{	
	int	div;
	int	mod;

	div = duration / SLEEP_STEP;
	mod = duration % SLEEP_STEP;
	while (div--)
	{
		if (am_i_dead(philo, -1) == YES_IM_SORRY || philo->env->living == DEAD)
			return (DIED);
		usleep(SLEEP_STEP * 1000);
	}
	if (am_i_dead(philo, -1) == YES_IM_SORRY || philo->env->living == DEAD)
		return (DIED);
	if (mod)
		usleep(mod * 1000);
	return (0);
}

//Dangers : le thread s'arrête en attendant que le mutex soit libre
//donc si un autre thread meurt, comment dire à celui ci d'arrêter d'attendre
//et de quitter ?
void	wanna_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	if (am_i_dead(philo, -1) == YES_IM_SORRY)
	{
		pthread_mutex_unlock(philo->right_fork);
		return ;
	}
	print_new_status(philo->timestamp, philo->str_id, TAKE_FORK,
			philo->status_buf);
	pthread_mutex_lock(philo->left_fork);
	if (am_i_dead(philo, -1) == YES_IM_SORRY)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	print_new_status(philo->timestamp, philo->str_id, TAKE_FORK,
			philo->status_buf);
	philo->state = EATING;
	print_new_status(philo->timestamp, philo->str_id, EATING,
			philo->status_buf);
	sleep_but_listen(philo, philo->env->tte);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	if (philo->state != DIED)
		philo->state = SLEEPING;
}

void	going_to_sleep(t_philo *philo)
{
	sleep_but_listen(philo, philo->env->tts);
	if (philo->state != DIED)
		philo->state = THINKING;
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (philo->env->living == ALL_ALIVE)
	{
		if (am_i_dead(philo, -1) == YES_IM_SORRY)
			break ;
		if (philo->state == THINKING)
		{
			print_new_status(philo->timestamp, philo->str_id, THINKING,
					philo->status_buf);
			wanna_eat(philo);//try to find forks, eat then set status to sleeping
		}
		else if (philo->state == SLEEPING)
		{
			print_new_status(philo->timestamp, philo->str_id, SLEEPING,
					philo->status_buf);
			going_to_sleep(philo);//sleep & check if philo is died. Then stat think
		}
	}
	return (NULL);
}

/*
		usleep(344910);
		get_timestamp(philo->time_start, &(philo->timestamp));
		print_new_status(philo->timestamp, philo->str_id, TAKE_FORK,
			philo->status_buf);

*/

int	launch_thread(t_env *env, pthread_t *thread, t_philo *philo)
{
	struct timeval	start_time_value;
	int				i;

	gettimeofday(&start_time_value, NULL);
	i = -1;
	while (++i < env->philo_nb)
	{
		usleep(4060);
		philo[i].time_start = start_time_value;
		pthread_create(&(thread[i]), NULL, routine, &(philo[i]));
	}
	return (0);
}

int	exit_thread(t_env *env, pthread_t *thread)
{
	int	i;

	i = -1;
	while (++i < env->philo_nb)
		pthread_join(thread[i], NULL);
	return (0);
}

int	main(int ac, char *av[])
{
	t_env			*env;
	t_philo			*philo;
	pthread_t		*thread;
	pthread_mutex_t	*mutex;

	set_struct_null(&philo, &env, &thread, &mutex);
	if (main_init(&philo, &env, ac, av) == -1)
		return (main_exit(philo, env, thread, mutex));
	if (thread_mutex_init(env, &thread, &mutex) == -1)
		return (main_exit(philo, env, thread, mutex));
	philo_init(philo, env, mutex);
	launch_thread(env, thread, philo);
	exit_thread(env, thread);
	free(thread);
	return (0);
}

/*

	print_env(env);
	print_thread(env, thread);
	system("leaks philo_one");

	printf("`````\n--now_sec: %ld---\n--now_u_sec: %ld---\n---time: %ld---\n\
	\n----timestamp: %ld----\n----timestamp_ms: %ld----\n\n",
	(long)(now.tv_sec), (long)(now.tv_usec), time,
	*time_stamp, (*time_stamp / 1000));
	fflush(stdout);

	printf("start_sec: %ld\tstart_usec: %ld\n", (long)(tv.tv_sec),
			(long)(tv.tv_usec));


		printf("strid: |%s|\n", philo[i].str_id);
		fflush(stdout);
*/
