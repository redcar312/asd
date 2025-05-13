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

typedef struct s_host
{
	struct s_philo		*philos;
	struct s_tc			t_count;
	pthread_mutex_t		*forks;
	long				fc;
	long				tl;
	long				sc;
	long				tc;
	long long			start_time;
	long long			time_to_die;
	long long			time_to_sleep;
	long long			time_to_eat;
	long				n_of_eats;
	long				n;
	pthread_t		monitor;
	pthread_mutex_t		status_lock;
	pthread_mutex_t	t_lock;
	bool	is_over;
	

}	s_host;

typedef struct	s_tc
{
	long	p_count;
	bool	monitor;
}	s_tc;

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

long long	ft_atoll(const char *num);
int	ft_atol(const char *str);
int	ft_atoi(const char *str);
int	f_isdigit(unsigned int c);
bool	validator(char **argv);
void	init_philo_data(struct s_host *host, pthread_mutex_t *forks, long n);
void	init_host_counters(struct s_host *host);
int	init_host_data(struct s_host *host, char **argv);
void	init_forks(struct s_host *host, long n);
void	*philo_loop(void *arg);
void	*monitor(void *arg);
bool	check_state(struct s_host *h);
void	set_status(struct s_host *h, bool status);
bool	has_died(struct s_philo *p);
bool	is_done(struct s_host *h);
void	write_status(struct s_philo *p, char *str);
void	sync_start(long long timer, struct s_host *host);
void	waiter(long long timer, struct s_philo *p);
long long	get_time(struct s_host *h);
void	handle_err(struct s_host *h, char *msg);
void	handle_lock(pthread_mutex_t *ptr, struct s_host *h);
void	handle_unlock(pthread_mutex_t *ptr, struct s_host *h);
#endif
