#include "philo_one.h"

int			sleep_but_listen(t_philo *philo, int duration)
{	
	long	timestamp;
	long	time_end;
	long	rest;
	int		first_part;

	timestamp = get_timestamp(philo->time_start);
	time_end = timestamp + (long)duration;
	first_part = duration * 80 / 100;
	usleep(first_part * 1000);
	timestamp = get_timestamp(philo->time_start);
	rest = time_end - timestamp;
	if (rest > 0)
		usleep(rest * 1000);
	if (am_i_dead(philo, -1) == YES_IM_SORRY)
		return (DIED);
	return (0);
}

/*
static int	micro_sleeps(t_philo *philo, int div, int mod, long time_end)
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

int			sleep_but_listen(t_philo *philo, int duration)
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
}*/
