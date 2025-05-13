/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:07:01 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/05/13 16:07:25 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_forks(struct s_host *host, long n)
{
	int	i;

	i = -1;
	if (pthread_mutex_init(&host->status_lock, NULL) != 0)
		handle_err(host, "fork init error");
	if (pthread_mutex_init(&host->t_lock, NULL) != 0)
		handle_err(host, "fork init error");
	while (++i < n)
	{
		if (pthread_mutex_init(&host->forks[i], NULL) != 0)
			handle_err(host, "fork init error");
		host->tc++;
	}
}

void	init_philo_data(struct s_host *host, pthread_mutex_t *forks, long n)
{
	long	i;

	i = -1;
	while (++i < n)
	{
		host->philos[i].host = host;
		host->philos[i].id = i + 1;
		host->philos[i].eat_counter = 0;
		if (pthread_mutex_init(&host->philos[i].lock, NULL) != 0)
			handle_err(host, "mutex init error");
		host->tl++;
		if (host->philos[i].id == (int)host->n)
		{
			    host->philos[i].l_fork = i;
			    host->philos[i].r_fork = 0;
		}
		else
		{
			if (i % 2 == 0)
			{
			    host->philos[i].l_fork = i + 1;
			    host->philos[i].r_fork = i;
			}
			else
			{
			    host->philos[i].l_fork = i;
			    host->philos[i].r_fork = i + 1;
			}
		}
	}
}

void	init_host_counters(struct s_host *host)
{
	host->fc = 0;
	host->tl = 0;
	host->sc = 0;
	host->tc = 0;
	host->t_count.monitor = false;
	host->t_count.p_count = 0;

}


int	init_host_data(struct s_host *host, char **argv)
{
	long	l = ft_atol(argv[1]);
	init_host_counters(host);
	host->philos = malloc(l * sizeof(s_philo));
	if (!host->philos)
		return (-1);
	host->forks = malloc(l * sizeof(pthread_mutex_t));
	if (!host->forks)
		handle_err(host, "malloc fail");
	init_forks(host, l);
	init_philo_data(host, host->forks, l);
	if (pthread_mutex_init(&host->status_lock, NULL) != 0)
			handle_err(host, "mutex init error");
	host->sc = 1;
	if (pthread_mutex_init(&host->t_lock, NULL) != 0)
			handle_err(host, "mutex init error");
	host->tc = 1;
	return (1);
}


