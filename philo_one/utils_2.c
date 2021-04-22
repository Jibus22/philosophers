/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 14:17:34 by jle-corr          #+#    #+#             */
/*   Updated: 2021/04/22 14:17:36 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

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

int		unlock_both_forks_exit(t_philo *philo, int *living)
{
	mtx_handler(UNLOCK, philo->mtx_rfork, living);
	mtx_handler(UNLOCK, philo->mtx_lfork, living);
	return (DEAD);
}
