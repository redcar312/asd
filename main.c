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
	unsigned int	l_fork;
	unsigned int	r_fork;
	struct s_host	*host;
	long long	last_eat;
	long	eat_counter;

}	s_philo;

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



long long	get_time(void)
{
	struct timeval	t;
	long long	res;

	gettimeofday(&t, NULL);
	res = (long long)(t.tv_sec * 1000) + (long long)(t.tv_usec / 1000);
	return (res);

}

void	write_status(struct s_philo *p, char *str)
{
	long long time;

	time = get_time();
	printf("%lld %d %s\n", time, p->id, str);
}

void	init_forks(struct s_host *host, long n)
{
	int	i;

	i = -1;

	while (++i < n)
	{
		if (pthread_mutex_init(&host->forks[i], NULL) != 0)
			printf("b\n");
	}
}

void	eat(s_philo *p)
{
	pthread_mutex_lock(&p->host->forks[p->l_fork]),
	pthread_mutex_lock(&p->host->forks[p->r_fork]);
	write_status(p, "has taken a fork");
	write_status(p, "is eating");
	usleep(1000);
	p->eat_counter++;
	pthread_mutex_unlock(&p->host->forks[p->l_fork]);
	pthread_mutex_unlock(&p->host->forks[p->r_fork]);
}

void	*philo_loop(void *arg)
{
	s_philo *p;

	p = (s_philo *)arg;
	while(get_time() < p->host->time_to_think);
	if(p->id % 2 == 0)
	{
		write_status(p, "is thinking");
		usleep(1000);
	}
	while(p->eat_counter < 5)
	{
		eat(p);
		write_status(p, "is sleeping");
		usleep(1000);
		write_status(p, "is thinking");
		usleep(1000);
	}
	return (NULL);
}

void	start_sim(struct s_host *host)
{
	long	i;

	i = -1;
	host->time_to_think = get_time() + ((long long)host->n * 2 * 10);
	while (++i < host->n)
	{
		pthread_create(&host->philos[i].thread, NULL, *philo_loop, (void *)&host->philos[i]);
	}
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

	host->philos = malloc(l * sizeof(s_philo));
	if (!host->philos)
		return (-1);
	host->forks = malloc(l * sizeof(pthread_mutex_t));
	if (!host->forks)
		exit(1);
	init_forks(host, l);
	init_philo_data(host, host->forks, l);
	return (1);
}


int	main(int argc, char **argv)
{
	struct s_host host;
	host.n = ft_atol(argv[1]);
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