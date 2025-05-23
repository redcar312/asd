/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhurtamo <mhurtamo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:23:13 by mhurtamo          #+#    #+#             */
/*   Updated: 2025/05/13 16:23:25 by mhurtamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_err(struct t_host *h, char *msg)
{	
	long	i;
	
	i = -1;
	print_error(msg);
	h->is_over = true;
	free(h->philos);
	free(h->forks);
	exit(1);
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
