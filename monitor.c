/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:13:42 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/05/13 16:13:59 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	has_died(struct t_philo *p)
{
	long long	time;
	bool		res;

	time = get_time(p->host);
	res = false;
	if ((time - p->last_eat) >= p->host->time_to_die)
	{
		res = true;
	}
	return (res);
}

bool	is_done(struct t_host *h)
{
	long	i;
	bool	ate_all;

	i = -1;
	ate_all = true;
	while (++i < h->n)
	{
		if (has_died(&h->philos[i]))
		{
			if (h->philos[i].eat_counter != h->n_of_eats)
			{
				write_status(&h->philos[i], "has died");
				h->is_over = true;
				return (true);
			}
		}
		if (h->n_of_eats != -1)
		{
			if (h->philos[i].eat_counter < h->n_of_eats)
				return (false);
		}
	}
	if (h->n_of_eats != -1 && ate_all)
	{
		h->is_over = true;
		return (true);
	}
	return (false);
}

void	*monitor(void *arg)
{
	t_host	*host;

	host = (t_host *)arg;
	sync_start(host->start_time, host);
	while (1)
	{
		if (is_done(host) || host->is_over)
			return(NULL);
	}
	return (NULL);
}
