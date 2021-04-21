/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoimax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-corr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 14:55:54 by jle-corr          #+#    #+#             */
/*   Updated: 2021/04/21 14:55:55 by jle-corr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

/*
** Convert string to int.
** The string may begin with an arbitrary amount of white space
** (as determined by ft_isspace) followed by a single optional `+' or `-' sign.
*/

int	ft_strtoimax(const char *str, char **endptr)
{
	unsigned long long	nbr;
	char				neg;
	short int			i;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	neg = 0;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			neg = 1;
	nbr = 0;
	while (ft_isdigit(str[i]))
		nbr = nbr * 10 + str[i++] - 48;
	*endptr = (char *)(&str[i]);
	if ((neg && nbr - 1 > INT_MAX) || (!neg && nbr > INT_MAX))
	{
		errno = ERANGE;
		return (0);
	}
	if (neg)
		return ((int)(-nbr));
	else
		return ((int)nbr);
}
