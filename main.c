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

void	handle_lock(pthread_mutex_t *ptr, struct s_host *h)
{
	if (pthread_mutex_lock(ptr) != 0)
		handle_err(h, "Mutex lock error");
}

void	handle_unlock(pthread_mutex_t *ptr, struct s_host *h)
{
	if (pthread_mutex_unlock(ptr) != 0)
		handle_err(h, "Mutex lock error");
}

void	start_sim(struct s_host *host)
{
	long	i;
	long long	delay;
	i = -1;
	delay = (long long)host->n * 2 * 10;
	host->start_time = get_time(host) + delay;
	while (++i < host->n)
	{
		if (pthread_create(&host->philos[i].thread, NULL, *philo_loop, (void *)&host->philos[i]) == -1)
			handle_err(host, "thread creation error");
		host->t_count.p_count++;
	}
	if (pthread_create(&host->monitor, NULL, *monitor, (void *)host) == -1)
		handle_err(host, "thread creation error");
	
}


int	main(int argc, char **argv)
{
	if (argc != 5 || argc != 6)
		exit(1);
	struct s_host host;
	host.n = ft_atol(argv[1]);
	host.time_to_eat = 1000;
	host.time_to_sleep = 1000;
	host.time_to_die = 5000;
	host.n_of_eats = -1;
	init_host_data(&host, argv);
	start_sim(&host);
    while(1);
}
