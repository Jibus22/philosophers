/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 14:56:06 by jle-corr          #+#    #+#             */
/*   Updated: 2021/04/22 14:41:51 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int			thread_init(t_env *env, pthread_t **thread)
{
	*thread = (pthread_t *)malloc(sizeof(pthread_t) * env->philo_nb);
	if (!*thread)
		return (FAIL_INIT);
	return (0);
}

int			philo_init(t_philo *philo, t_env *env)
{
	t_philo			*philo_cpy;
	char			*temp_id;
	int				i;

	i = -1;
	while (++i < env->philo_nb)
	{
		philo_cpy = &(philo[i]);
		philo_cpy->env = env;
		philo_cpy->id = i + 1;
		temp_id = ft_itoa(philo_cpy->id);
		if (!temp_id)
			return (FAIL_INIT);
		philo_cpy->str_id[0] = ' ';
		ft_strlcpy(&(philo_cpy->str_id[1]), temp_id, 5);
		free(temp_id);
		philo_cpy->state = THINKING;
		philo_cpy->sem_forks = env->sem_forks;
	}
	return (0);
}
