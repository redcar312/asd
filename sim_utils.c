/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:18:25 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/05/13 16:18:47 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(struct t_host *h)
{
	struct timeval	t;
	long long		res;
	if (gettimeofday(&t, NULL) == -1)
		handle_err(h, "time err");
	res = (long long)(t.tv_sec * 1000) + (long long)(t.tv_usec / 1000);
	return (res);
}

void	test(t_philo *p, long long timer)
{
	long long	start;
	
	start = get_time(p->host);
	while ((get_time(p->host) - start) < timer)
		usleep(100);
}

void	waiter(long long timer, struct t_philo *p)
{
	long long	time;
	
	time = get_time(p->host);
	if (p->host->is_over)
	{
		take_locks(p);
		return ;
	}
	while (get_time(p->host) < (time + timer))
	{	
		if (p->host->is_over)
		{
			take_locks(p);
			return ;
		}
		else
			test(p, 10);
	}
}

void	sync_start(long long timer, struct t_host *host)
{
	while (get_time(host) < timer)
		usleep(1);
}

void	write_status(struct t_philo *p, char *str)
{
	long long	time;
	handle_lock(&p->host->t_lock, p->host);
	p->wl = true;
	time = get_time(p->host);
	if (p->host->is_over)
	{
		take_locks(p);
		return ;
	}
	printf("%lld %ld %s\n", time, p->id, str);
	handle_unlock(&p->host->t_lock, p->host);
	p->wl = false;
}
