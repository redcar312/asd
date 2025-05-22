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

void	take_locks(t_philo *p)
{
	if(p->lf)
		handle_unlock(&p->host->forks[p->l_fork], p->host);
	if (p->rf)
		handle_unlock(&p->host->forks[p->r_fork], p->host);
	if (p->wl)
		handle_unlock(&p->host->t_lock);
}

static void	eat(t_philo *p)
{
	if (p->host->is_over || p->eat_counter == p->host->n_of_eats)
		return;
	handle_lock(&p->host->forks[p->l_fork], p->host);
	p->lf = true;
	handle_lock(&p->host->forks[p->r_fork], p->host);
	p->rf = true;
	write_status(p, "has taken a fork");
	write_status(p, "is eating");
	p->last_eat = get_time(p->host);
	waiter(p->host->time_to_eat, p);
	p->eat_counter++;
	handle_unlock(&p->host->forks[p->l_fork], p->host);
	p->lf = false;
	handle_unlock(&p->host->forks[p->r_fork], p->host);
	p->rf = false;
	if (p->host->is_over || p->eat_counter == p->host->n_of_eats)
		return;
	write_status(p, "is sleeping");
	waiter(p->host->time_to_sleep, p);
}

static void	think(struct t_philo *p, bool is_silent)
{
	long long	ttt;

	ttt = p->host->time_to_die;
	ttt = (ttt - (get_time(p->host) - p->last_eat) - p->host->time_to_eat) / 2;
	if (ttt < 0)
		ttt = 0;
	if (ttt > 600)
		ttt = 200;
	if (p->host->is_over || p->eat_counter == p->host->n_of_eats)
		return;
	if (!is_silent)
		write_status(p, "is thinking");
	waiter(ttt, p);
}

void	*single_philo(void *arg)
{
	t_philo		*p;
	long long	t;

	p = (t_philo *)arg;
	t = get_time(p->host) + p->host->time_to_die;
	write_status(p, "has taken a fork");
	while (get_time(p->host) < t)
	{
		usleep(1);
	}
	write_status(p, "has died");
	return (NULL);
}

void	*philo_loop(void *arg)
{
	t_philo	*p;
	p = (t_philo *)arg;
	p->last_eat = p->host->start_time;
	sync_start(p->host->start_time, p->host);
	if (p->id % 2 == 0)
		think(p, true);
	while (!p->host->is_over)
	{
		eat(p);
		think(p, false);
	}
	take_locks(p);
	return (NULL);
}
