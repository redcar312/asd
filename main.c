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

int	f_isdigit(unsigned int c)
{
	if (c <= 57 && c >= 48)
		return (1);
	else
		return (0);
}

int	f_isxdigit(unsigned int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else if (c >= 'A' && c <= 'F')
		return (1);
	else if (c >= 'a' && c <= 'f')
		return (1);
	return (0);
}

static int	hexval(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	else if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (0);
}

static long	ft_atol(const char *num)
{
	long	value;
	int		neg;

	value = 0;
	neg = 0;
	if (num[0] == '0' && num[1] == 'x')
	{
		num += 2;
		while (*num && f_isxdigit(*num))
			value = value * 16 + hexval(*num++);
	}
	else
	{
		if (num[0] == '-')
		{
			neg = 1;
			num++;
		}
		while (*num && f_isdigit(*num))
			value = value * 10 + *num++ - '0';
	}
	if (neg)
		value = -value;
	return (value);
}

int	ft_atoi(const char *str)
{
	return (ft_atol(str));
}

void	handle_mutex_removal(struct s_host *h)
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

void	handle_free(struct s_host *h)
{
	long	i;
	
	i = 0;
	if (!h)
		return;
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
void	handle_thread_removal(struct s_host *host)
{
	long	i;
	

}

void	handle_err(struct s_host *h, char *msg)
{
	if (!h)
		exit(1);
	handle_thread_removal(h);
	handle_mutex_removal(h);
	handle_free(h);
	print_error(msg);
	exit(1);
}

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

long long	get_time(struct s_host *h)
{
	struct timeval	t;
	long long	res;

	if (gettimeofday(&t, NULL) == -1)
		handle_err(h, "gettime error");
	res = (long long)(t.tv_sec * 1000) + (long long)(t.tv_usec / 1000);
	return (res);
}

bool	check_state(struct s_host *h)
{
	bool	state;

	pthread_mutex_lock(&h->status_lock);
	state = h->is_over;
	pthread_mutex_unlock(&h->status_lock);
	return (state);
}

void	waiter(long long timer, struct s_philo *p)
{
	long long	time;

	time = get_time(p->host);
	while (get_time(p->host) < (time + timer) && !check_state(p->host))
	{
		usleep(50);
	}
	return ;
}

void	sync_start(long long timer, struct s_host *host)
{
	while (get_time(host) < timer)
		usleep(1);
}

void	write_status(struct s_philo *p, char *str)
{
	long long time;
    
    handle_lock(&p->host->t_lock, p->host);
	time = get_time(p->host);
	printf("%lld %d %s\n", time, p->id, str);
	handle_unlock(&p->host->t_lock, p->host);
}

void	init_forks(struct s_host *host, long n)
{
	int	i;

	i = -1;
	if (pthread_mutex_init(&host->status_lock, NULL) != 0)
		handle_err(host, "fork init error");
	if (pthread_mutex_init(&host->t_lock, NULL) != 0)
		handle_err(host, "fork init error");
	while (++i < n)
	{
		if (pthread_mutex_init(&host->forks[i], NULL) != 0)
			handle_err(host, "fork init error");
		host->tc++;
	}
}

void	eat(s_philo *p)
{
	handle_lock(&p->host->forks[p->l_fork], p->host);
	handle_lock(&p->host->forks[p->r_fork], p->host);
	write_status(p, "has taken a fork");
	write_status(p, "is eating");
	handle_lock(&p->lock, p->host);
	p->last_eat = get_time(p->host);
    handle_unlock(&p->lock, p->host);
	waiter(p->host->time_to_eat, p);
	p->eat_counter++;
	handle_unlock(&p->host->forks[p->l_fork], p->host);
	handle_unlock(&p->host->forks[p->r_fork], p->host);
}

void	think(struct s_philo *p)
{
	long long ttt;

	ttt = p->host->time_to_die;
	ttt = (ttt - (get_time(p->host) - p->last_eat) - p->host->time_to_eat) / 2;

	if (ttt < 0)
		ttt = 0;
	if (ttt > 600)
		ttt = 200;
	write_status(p, "is thinking");
	waiter(ttt, p);
}

void	p_sleep(struct s_philo *p)
{
	write_status(p, "is sleeping");
	waiter(p->host->time_to_sleep, p);
}


void	set_status(struct s_host *h, bool status)
{
	pthread_mutex_lock(&h->status_lock);
	h->is_over = status;
	pthread_mutex_unlock(&h->status_lock);
}

bool	has_died(struct s_philo *p)
{
	long long	time;
	bool	res;

    handle_lock(&p->lock, p->host);
	time = get_time(p->host);
	res = false;
	if ((time - p->last_eat) >= p->host->time_to_die)
		res = true;
	handle_unlock(&p->lock, p->host);
	return (res);
}

bool	is_done(struct s_host *h)
{
	long	i;
	bool	ate_all;
	long	en;
	
	i = -1;
	ate_all = true;
	en = h->n_of_eats;
	while (++i < h->n)
	{
		if (has_died(&h->philos[i]))
		{
			set_status(h, true);
			return (true);
		}
		if (h->n_of_eats != -1)
		{
			if(h->philos[i].eat_counter < en)
				return (false);
		}
	}
	if (en != -1 && ate_all)
	{
		set_status(h, true);
		return (true);
	}
	return (false);
}

void	*philo_loop(void *arg)
{
	s_philo *p;

	p = (s_philo *)arg;
	handle_lock(&p->lock, p->host);
	p->last_eat = p->host->start_time;
	handle_unlock(&p->lock, p->host);
	sync_start(p->host->start_time, p->host);
	if (p->id % 2 == 0)
		waiter(10, p);
	while (1)
	{
		eat(p);
		p_sleep(p);
		think(p);
	}
	return (NULL);
}

void	*monitor(void *arg)
{
	s_host	*host;

	host = (s_host *)arg;
	sync_start(host->start_time, host);
	while(1)
	{
		if(is_done(host))
			return (NULL);
	}
	return (NULL);
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
	}
	if (pthread_create(&host->monitor, NULL, *monitor, (void *)host) == -1)
		handle_err(host, "thread creation error");
	
}

void	init_philo_data(struct s_host *host, pthread_mutex_t *forks, long n)
{
	long	i;

	i = -1;
	while (++i < n)
	{
		host->philos[i].host = host;
		host->philos[i].id = i + 1;
		host->philos[i].eat_counter = 0;
		if (pthread_mutex_init(&host->philos[i].lock, NULL) != 0)
			handle_err(host, "mutex init error");
		host->tl++;
		if (host->philos[i].id == (int)host->n)
		{
			    host->philos[i].l_fork = i;
			    host->philos[i].r_fork = 0;
		}
		else
		{
			if (i % 2 == 0)
			{
			    host->philos[i].l_fork = i + 1;
			    host->philos[i].r_fork = i;
			}
			else
			{
			    host->philos[i].l_fork = i;
			    host->philos[i].r_fork = i + 1;
			}
		}
	}
}

int	init_host_data(struct s_host *host, char **argv)
{
	long	l = ft_atol(argv[1]);
	host->fc = 0;
	host->tl = 0;
	host->sc = 0;
	host->tc = 0;
	host->philos = malloc(l * sizeof(s_philo));
	if (!host->philos)
		return (-1);
	host->forks = malloc(l * sizeof(pthread_mutex_t));
	if (!host->forks)
		handle_err(host, "malloc fail");
	init_forks(host, l);
	init_philo_data(host, host->forks, l);
	if (pthread_mutex_init(&host->status_lock, NULL) != 0)
			handle_err(host, "mutex init error");
	host->sc = 1;
	if (pthread_mutex_init(&host->t_lock, NULL) != 0)
			handle_err(host, "mutex init error");
	host->tc = 1;
	return (1);
}

int	main(int argc, char **argv)
{
//join threads on destroy;
	struct s_host host;
	host.n = ft_atol(argv[1]);
	host.time_to_eat = 1000;
	host.time_to_sleep = 1000;
	host.time_to_die = 5000;
	host.n_of_eats = -1;
	/*struct s_philo p;
	struct s_fork f1;
	struct s_fork f2;
	pthread_t t;
	p.thread = t;
	p.eat_counter = 0;
	p.l_fork = f1;
	p.r_fork = f2;
	p.id = 1;
	pthread_mutex_init(&f1.lock, NULL);
	pthread_mutex_init(&f2.lock, NULL);
	pthread_create(&p.thread, NULL, *philo_loop, (void *)&p);
	struct s_philo p2;
	struct s_fork f11;
	struct s_fork f22;
	pthread_t t1;
	p2.thread = t1;
	p2.eat_counter = 0;
	p2.l_fork = f1;
	p2.r_fork = f2;
	p2.id = 2;
	usleep(1000);
	pthread_create(&p2.thread, NULL, *philo_loop, (void *)&p2);
	while(1);
	*/
	init_host_data(&host, argv);
	start_sim(&host);
    while(1);
}