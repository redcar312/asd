/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:37:22 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/05/07 15:37:43 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_lock(pthread_mutex_t *ptr, struct t_host *h)
{
	if (pthread_mutex_lock(ptr) != 0)
		handle_err(h, "Mutex lock error");
}

void	handle_unlock(pthread_mutex_t *ptr, struct t_host *h)
{
	if (pthread_mutex_unlock(ptr) != 0)
		handle_err(h, "Mutex lock error");
}

static void	handle_single(struct t_host *host)
{
	if (pthread_create(&host->philos[0].thread, NULL,
			*single_philo, (void *)&host->philos[0]) != 0)
	{
		exit(1);
	}	
	host->t_count.p_count++;
}

void	delete_all(t_host *host, long count, bool m)
{
	long	i;

	i = -1;
	if (m)
		pthread_join(host->monitor, NULL);
	while (++i < count)
		pthread_join(host->philos[i].thread, NULL);
	free(host->philos);
	free(host->forks);
	exit(1);
		
}

void	start_sim(struct t_host *host)
{
	long		i;
	long long	delay;

	i = -1;
	delay = (long long)host->n * 2 * 10;
	host->start_time = get_time(host) + delay;
	while (++i < host->n)
	{
		if (pthread_create(&host->philos[i].thread, NULL, *philo_loop, (void *)&host->philos[i]) != 0)
		{
			print_error("thread creation error");
			delete_all(host, i, false);
		}	
		host->t_count.p_count++;
	}
	if (pthread_create(&host->monitor, NULL, *monitor, (void *)host) != 0)
	{
		print_error("thread creation error");
		delete_all(host, i, true);
	}
	i = -1;
	pthread_join(host->monitor, NULL);
	while (++i < host->n)
		pthread_join(host->philos[i].thread, NULL);
	free(host->philos);
	free(host->forks);
}

int	main(int argc, char **argv)
{
	struct t_host	host;

	if (argc < 5 || argc > 6)
	{
		print_error("invalid number of args");
		exit(1);
	}
	if (!validator(argv))
	{
		print_error("invalid arguments");
		handle_single(&host);
		exit(1);
	}
	host.n = ft_atol(argv[1]);
	host.is_over = false;
	init_host_data(&host, argv);
	start_sim(&host);
	while (1)
	{
		if (host.is_over)
			break;
	}
	return(0);
}
