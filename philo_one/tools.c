#include "philo_one.h"

int	print_thread(t_env *env, pthread_t *thread)
{
	int	i;

	i = 0;
	while (++i < (env->philo_nb + 1))
	{
		printf("thread[%d]: %lu\n", i, (unsigned long)thread[i]);
		fflush(stdout);
	}
	return (0);
}

void	print_env(t_env *env)
{
	printf("philo_nb: %6d\tttd: %6d\ttte: %6d\ttts: %6d\nfork_nb: %6d\n",
			env->philo_nb, env->ttd, env->tte, env->tts, env->fork_nb);
}
