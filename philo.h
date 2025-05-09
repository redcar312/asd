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
	long	fc;
	long	tl;
	long	sc;
	long	tc;
	long long	start_time;
	long long	time_to_die;
	long long	time_to_sleep;
	long long	time_to_eat;
	long	n_of_eats;
	long	n;
	pthread_t	monitor;
	pthread_mutex_t	status_lock;
	pthread_mutex_t	t_lock;
	bool	is_over;
	

}	s_host;

typedef struct s_philo
{
	int	id;
	pthread_t	thread;
	unsigned int	l_fork;
	unsigned int	r_fork;
	struct s_host	*host;
	long long	last_eat;
	long	eat_counter;
	pthread_mutex_t	lock;
}	s_philo;


#endif
