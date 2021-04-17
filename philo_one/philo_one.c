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

int	micro_sleeps(t_philo *philo, int div, int mod, long time_end)
{
	long	timestamp;
	long	end_of_sleep;

	while (div--)
	{
		usleep(SLEEP_STEP * 1000);
		if (div < 3)
		{
			timestamp = get_timestamp(philo->time_start);
			end_of_sleep = time_end - timestamp;
			if (end_of_sleep < SLEEP_STEP)
			{
				usleep(end_of_sleep * 1000);
				div = 0;
				mod = 0;
				return (0);
			}
		}
		if (philo->env->living == DEAD)
			return (-1);
	}
	if (mod)
		usleep(mod * 1000);
	return (0);
}

int	sleep_but_listen(t_philo *philo, int duration)
{	
	int		div;
	int		mod;
	long	timestamp;
	long	time_end;
	long	dead_limit;

	dead_limit = philo->last_lunch + (long)philo->env->ttd;
	timestamp = get_timestamp(philo->time_start);
	time_end = timestamp + duration;
	if (time_end > dead_limit)
	{
		duration = (int)dead_limit - (int)timestamp;
		time_end = dead_limit;
		div = duration / SLEEP_STEP;
		mod = duration % SLEEP_STEP;
	}
	else
	{
		div = duration / SLEEP_STEP;
		mod = duration % SLEEP_STEP;
	}
	micro_sleeps(philo, div, mod, time_end);
	if (am_i_dead(philo, -1) == YES_IM_SORRY)
		return (DIED);
	return (0);
}

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

//Dangers : le thread s'arrête en attendant que le mutex soit libre
//donc si un autre thread meurt, comment dire à celui ci d'arrêter d'attendre
//et de quitter ?
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
	//philo->last_lunch = get_timestamp(philo->time_start);
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
		if (am_i_dead(philo, -1) == YES_IM_SORRY)
			break ;
		if (philo->state == THINKING)
		{
			print_new_status(philo, philo->str_id, THINKING, philo->status_buf);
			wanna_eat(philo);//try to find forks, eat then set status to sleeping
		}
		else if (philo->state == SLEEPING)
		{
			print_new_status(philo, philo->str_id, SLEEPING, philo->status_buf);
			going_to_sleep(philo);//sleep & check if philo is died. Then stat think
		}
	}
	return (NULL);
}

/*
		usleep(344910);
		get_timestamp(philo->time_start, &(philo->timestamp));
		print_new_status(philo, philo->str_id, TAKE_FORK,
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
		usleep(1060);
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
