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

void	init_forks(struct t_host *host, long n)
{
	long	i;

	i = -1;
	if (pthread_mutex_init(&host->t_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&host->t_lock);
		free(host->philos);
		print_error("fork creation error");
		exit(1);
	}
	while (++i < n)
	{
		if (pthread_mutex_init(&host->forks[i], NULL) != 0)
		{
			while (i > 0)
			{
				pthread_mutex_destroy(&host->forks[i]);
				i--;
			}
			free(host->philos);
			print_error("fork creation error");
			exit(1);
		}
	}
}

static void	give_forks(struct t_philo *philo, long i)
{
	if (philo->id == philo->host->n)
	{
		philo->l_fork = i;
		philo->r_fork = 0;
	}
	else
	{
		if (i % 2 == 0)
		{
			philo->l_fork = i + 1;
			philo->r_fork = i;
		}
		else
		{
			philo->l_fork = i;
			philo->r_fork = i + 1;
		}
	}
}

void	init_p_data(struct t_host *host, long n)
{
	long	i;

	i = -1;
	while (++i < n)
	{
		host->philos[i].host = host;
		host->philos[i].id = i + 1;
		host->philos[i].eat_counter = 0;
		host->philos[i].rf = false;
		host->philos[i].lf = false;	
		host->philos[i].wl = false;
		give_forks(&host->philos[i], i);
	}
}

static void	init_host_counters(struct t_host *host, char **argv)
{
	host->fc = 0;
	host->tl = 0;
	host->sc = 0;
	host->tc = 0;
	host->t_count.monitor = false;
	host->t_count.p_count = 0;
	host->time_to_die = ft_atoll(argv[2]);
	host->time_to_eat = ft_atoll(argv[3]);
	host->time_to_sleep = ft_atoll(argv[4]);
	host->n_of_eats = -1;
	if (argv[5])
	{
		host->n_of_eats = ft_atol(argv[5]);
	}
}

int	init_host_data(struct t_host *host, char **argv)
{
	long	l;

	l = ft_atol(argv[1]);
	init_host_counters(host, argv);
	host->philos = malloc(l * sizeof(t_philo));
	if (!host->philos)
		return (-1);
	host->forks = malloc(l * sizeof(pthread_mutex_t));
	if (!host->forks)
	{
		free(host->philos);
		return (-1);
	}
	init_forks(host, l);
	init_p_data(host, l);
	host->sc = 1;
	if (pthread_mutex_init(&host->t_lock, NULL) != 0)
		handle_err(host, "mutex init error");
	host->tc = 1;
	return (1);
}
