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
# include <errno.h>


typedef struct t_tc
{
	long	p_count;
	bool	monitor;
}	t_tc;

typedef struct t_host
{
	struct t_philo		*philos;
	struct t_tc			t_count;
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
	pthread_mutex_t		t_lock;
	_Atomic bool				is_over;
}	t_host;

typedef struct t_philo
{
	long				id;
	pthread_t			thread;
	long				l_fork;
	long				r_fork;
	_Atomic bool	lf;
	_Atomic bool	rf;
	_Atomic bool	wl;
	struct t_host		*host;
	_Atomic long long			last_eat;
	_Atomic long				eat_counter;
	pthread_mutex_t		lock;
}	t_philo;

long long	ft_atoll(const char *num);
int			ft_atol(const char *str);
int			ft_atoi(const char *str);
int			f_isdigit(unsigned int c);
int			handle_flock(pthread_mutex_t *ptr, t_philo *p, int n);
bool		validator(char **argv);
void		init_p_data(struct t_host *host, long n);
int			init_host_data(struct t_host *host, char **argv);
void		init_forks(struct t_host *host, long n);
void		*philo_loop(void *arg);
void		*monitor(void *arg);
bool		check_state(struct t_host *h);
void		set_status(struct t_host *h, bool status);
bool		has_died(struct t_philo *p);
bool		is_done(struct t_host *h);
void		write_status(struct t_philo *p, char *str);
void		sync_start(long long timer, struct t_host *host);
void		waiter(long long timer, struct t_philo *p);
long long	get_time(struct t_host *h);
void		handle_err(struct t_host *h, char *msg);
void		handle_lock(pthread_mutex_t *ptr, struct t_host *h);
void		handle_unlock(pthread_mutex_t *ptr, struct t_host *h);
void		print_error(char *msg);
void		*single_philo(void *arg);
void		handle_mutex_removal(struct t_host *h);
void		handle_free(struct t_host *h);
void		handle_thread_removal(struct t_host *host);
void		end_sim(struct t_host *h);
void		take_locks(t_philo *p);
#endif
