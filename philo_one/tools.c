/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 14:57:14 by jle-corr          #+#    #+#             */
/*   Updated: 2021/04/21 14:57:15 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int		print_thread(t_env *env, pthread_t *thread)
{
	int	i;

	i = -1;
	while (++i < env->philo_nb)
	{
		printf("thread[%d]: %lu\n", i, (unsigned long)thread[i]);
		fflush(stdout);
	}
	return (0);
}

void	print_env(t_env *env)
{
	printf("philo_nb: %6d\tttd: %6d\ttte: %6d\ttts: %6d\tph_living: %d\n",
	env->philo_nb, env->ttd, env->tte, env->tts, env->living);
}
