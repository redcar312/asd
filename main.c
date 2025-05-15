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
			*single_philo, (void *)&host->philos[0]) == -1)
		handle_err(host, "thread creation error");
	host->t_count.p_count++;
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
		if (pthread_create(&host->philos[i].thread, NULL,
				*philo_loop, (void *)&host->philos[i]) == -1)
			handle_err(host, "thread creation error");
		host->t_count.p_count++;
	}
	if (pthread_create(&host->monitor, NULL, *monitor, (void *)host) == -1)
		handle_err(host, "thread creation error");
	host->monitor = true;
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
		exit(1);
	}
	host.n = ft_atol(argv[1]);
	init_host_data(&host, argv);
	if (host.n == 1)
		handle_single(&host);
	else
		start_sim(&host);
	while(1);
	exit(0);
}
