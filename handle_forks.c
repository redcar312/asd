/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_forks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 16:03:23 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/05/22 16:12:23 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	handle_flock(pthread_mutex_t *ptr, t_philo *p, int n)
{
	if (n == 1 && p->host->is_over)
		return (1);
	if (n == 2 && p->host->is_over)
	{
		take_locks(p);
		return (1);
	}		
	if (pthread_mutex_lock(ptr) != 0)
		handle_err(p->host, "Mutex lock error");
	if (n == 1)
		p->lf = true;
	else
		p->rf = true;
		
	return (0);
}

void	take_locks(t_philo *p)
{
	if (p->lf)
	{
		handle_unlock(&p->host->forks[p->l_fork], p->host);
		p->lf = false;
	}
	if (p->rf)
	{
		handle_unlock(&p->host->forks[p->r_fork], p->host);
		p->rf = false;
	}
	if (p->wl)
	{
		handle_unlock(&p->host->t_lock, p->host);
		p->wl = false;
	}
}
