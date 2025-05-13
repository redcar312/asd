/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:59:23 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/05/13 15:59:46 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	hexval(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	else if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (0);
}

static int	f_isxdigit(unsigned int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else if (c >= 'A' && c <= 'F')
		return (1);
	else if (c >= 'a' && c <= 'f')
		return (1);
	return (0);
}

long long	ft_atoll(const char *num)
{
	long	value;
	int		neg;

	value = 0;
	neg = 0;
	if (num[0] == '0' && num[1] == 'x')
	{
		num += 2;
		while (*num && f_isxdigit(*num))
			value = value * 16 + hexval(*num++);
	}
	else
	{
		if (num[0] == '-')
		{
			neg = 1;
			num++;
		}
		while (*num && f_isdigit(*num))
			value = value * 10 + *num++ - '0';
	}
	if (neg)
		value = -value;
	return (value);
}

int	ft_atol(const char *str)
{
	return (ft_atol(str));
}

int	ft_atoi(const char *str)
{
	return (ft_atol(str));
}
