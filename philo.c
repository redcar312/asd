/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:09:59 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/05/13 16:10:13 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eat(s_philo *p)
{
	handle_lock(&p->host->forks[p->l_fork], p->host);
	handle_lock(&p->host->forks[p->r_fork], p->host);
	write_status(p, "has taken a fork");
	write_status(p, "is eating");
	handle_lock(&p->lock, p->host);
	p->last_eat = get_time(p->host);
	handle_unlock(&p->lock, p->host);
	waiter(p->host->time_to_eat, p);
	p->eat_counter++;
	handle_unlock(&p->host->forks[p->l_fork], p->host);
	handle_unlock(&p->host->forks[p->r_fork], p->host);
}

static void	think(struct s_philo *p)
{
	long long ttt;

	ttt = p->host->time_to_die;
	ttt = (ttt - (get_time(p->host) - p->last_eat) - p->host->time_to_eat) / 2;
	if (ttt < 0)
		ttt = 0;
	if (ttt > 600)
		ttt = 200;
	write_status(p, "is thinking");
	waiter(ttt, p);
}

static void	p_sleep(struct s_philo *p)
{
	write_status(p, "is sleeping");
	waiter(p->host->time_to_sleep, p);
}

void	*philo_loop(void *arg)
{
	s_philo *p;

	p = (s_philo *)arg;
	handle_lock(&p->lock, p->host);
	p->last_eat = p->host->start_time;
	handle_unlock(&p->lock, p->host);
	sync_start(p->host->start_time, p->host);
	if (p->id % 2 == 0)
		waiter(10, p);
	while (1)
	{
		eat(p);
		p_sleep(p);
		think(p);
	}
	return (NULL);
}
