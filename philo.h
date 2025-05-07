/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:38:23 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/05/07 15:38:26 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>
# include <stdbool.h>

typedef struct	s_host
{
	struct s_philo	*philos;
	pthread_mutex_t	*forks;
	long long	time_to_die;
	long long	time_to_sleep;
	long long	time_to_think;
	long	n_of_eats;
	long	n;
	pthread_t	monitor;
	pthread_mutex_t	monitor_lock;

}	s_host;

typedef struct s_philo
{
	int	id;
	pthread_t	thread;
	pthread_mutex_t	l_fork;
	pthread_mutex_t	r_fork;
	struct s_host	*host;
	long long	last_eat;
	long	eat_counter;

}	s_philo;

#endif
