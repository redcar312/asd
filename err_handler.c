/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:23:13 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/05/13 16:23:25 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_mutex_removal(struct t_host *h)
{
	long	i;

	i = 0;
	if (!h)
		return ;
	if (h->sc != 0)
		pthread_mutex_destroy(&h->status_lock);
	if (h->tc != 0)
		pthread_mutex_destroy(&h->t_lock);
	if (h->fc != 0)
	{
		while (i < h->fc)
		{
			pthread_mutex_destroy(&h->forks[i]);
			i++;
		}
	}
}

void	handle_free(struct t_host *h)
{
	long	i;

	i = 0;
	if (!h)
		return ;
	while (i < h->tl)
	{
		pthread_mutex_destroy(&h->philos[i].lock);
		i++;
	}
	free(h->philos);
}

void	print_error(char *msg)
{
	size_t	i;

	i = 0;
	if (!msg)
		return ;
	while (msg[i])
	{
		write(2, &msg[i], 1);
		i++;
	}
	write(2, "\n", 1);
}

void	handle_thread_removal(struct t_host *host)
{
	long	i;

	i = 0;
	while (i < host->t_count.p_count)
	{
		pthread_join(host->philos[i].thread, NULL);
		i++;
	}
	if (host->t_count.monitor)
		pthread_join(host->monitor, NULL);
	while (i < host->t_count.p_count)
	{
		pthread_exit((void *)&host->philos[i].thread);
		i++;
	}
	if (host->t_count.monitor)
		pthread_exit((void *)&host->monitor);
}

void	handle_err(struct t_host *h, char *msg)
{
	if (!h)
		exit(1);
	handle_thread_removal(h);
	handle_mutex_removal(h);
	handle_free(h);
	print_error(msg);
	exit(1);
}
