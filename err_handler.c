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

void	handle_err(struct t_host *h, char *msg)
{	
	long	n;
	
	n = 0;
	pthread_mutex_destroy(&h->t_lock);
	while (n < h->n)
	{
		take_locks(&h->philos[n]);
		pthread_mutex_destroy(&h->forks[n]);
		n++;
	}
	free(h->philos);
	print_error(msg);
	exit(1);
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
	pthread_exit(NULL);
}
