/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:04:47 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/05/13 16:05:04 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	f_isdigit(unsigned int c)
{
	if (c <= 57 && c >= 48)
		return (1);
	else
		return (0);
}

bool	validator(char **argv)
{
	size_t	i;
	size_t	j;

	if (!argv)
		return (false);
	i = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (!f_isdigit(argv[i][j]))
				return (false);
			j++;
		}
		if (ft_atoll(argv[i]) == 0)
			return (false);
		i++;
	}
	return (true);
}
