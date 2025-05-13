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

long long	get_time(struct s_host *h)
{
	struct timeval	t;
	long long	res;

	if (gettimeofday(&t, NULL) == -1)
		handle_err(h, "gettime error");
	res = (long long)(t.tv_sec * 1000) + (long long)(t.tv_usec / 1000);
	return (res);
}

void	waiter(long long timer, struct s_philo *p)
{
	long long	time;

	time = get_time(p->host);
	while (get_time(p->host) < (time + timer) && !check_state(p->host))
	{
		usleep(50);
	}
	return ;
}

void	sync_start(long long timer, struct s_host *host)
{
	while (get_time(host) < timer)
		usleep(1);
}

void	write_status(struct s_philo *p, char *str)
{
	long long time;
    
	handle_lock(&p->host->t_lock, p->host);
	time = get_time(p->host);
	printf("%lld %d %s\n", time, p->id, str);
	handle_unlock(&p->host->t_lock, p->host);
}

