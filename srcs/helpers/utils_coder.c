/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_coder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/08 11:45:31 by username          #+#    #+#             */
/*   Updated: 2026/11/02 16:37:29 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	verify_wait(t_dongles *dongles, t_coder *coder)
{
	if (!dongle_ok(dongles->small_dongle, coder))
	{
		pthread_mutex_unlock(&dongles->large_dongle->mutex);
		wait_dongle(dongles->small_dongle, coder);
	}
	else
	{
		pthread_mutex_unlock(&dongles->small_dongle->mutex);
		wait_dongle(dongles->large_dongle, coder);
	}
}

int	wait_dongles(t_dongles *dongles, t_coder *coder)
{
	if (dongle_ok(dongles->small_dongle, coder)
		&& dongles->large_dongle->is_busy == 1)
	{
		remove_top(dongles->small_dongle, coder->table);
		pthread_cond_broadcast(&dongles->small_dongle->cond);
		pthread_mutex_unlock(&dongles->small_dongle->mutex);
		wait_dongle(dongles->large_dongle, coder);
		return (1);
	}
	else if (dongle_ok(dongles->large_dongle, coder)
		&& dongles->small_dongle->is_busy == 1)
	{
		remove_top(dongles->large_dongle, coder->table);
		pthread_cond_broadcast(&dongles->large_dongle->cond);
		pthread_mutex_unlock(&dongles->large_dongle->mutex);
		wait_dongle(dongles->small_dongle, coder);
		return (2);
	}
	else
		verify_wait(dongles, coder);
	return (0);
}

void	take_dongles(t_dongles *dongles, t_coder *coder)
{
	remove_top(dongles->small_dongle, coder->table);
	remove_top(dongles->large_dongle, coder->table);
	dongles->small_dongle->is_busy = 1;
	dongles->large_dongle->is_busy = 1;
	log_state(coder->id, "has taken a dongle", coder->table);
	log_state(coder->id, "has taken a dongle", coder->table);
}

void	leave_dongle(t_dongle *dongle, long cooldown)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_busy = 0;
	dongle->available_at = current_time_ms() + cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

int	acquire_dongles(t_dongles *dongles, t_coder *coder)
{
	int		ticket;
	long	deadline;

	pthread_mutex_lock(&coder->table->log_mutex);
	ticket = coder->table->count_tickets;
	deadline = coder->table->time_burnout;
	coder->table->count_tickets++;
	pthread_mutex_unlock(&coder->table->log_mutex);
	pthread_mutex_lock(&coder->status_coder);
	deadline += coder->last_compile_start;
	pthread_mutex_unlock(&coder->status_coder);
	pthread_mutex_lock(&dongles->small_dongle->mutex);
	pthread_mutex_lock(&dongles->large_dongle->mutex);
	iniciate_request(coder, ticket, deadline, dongles);
	pthread_mutex_unlock(&dongles->small_dongle->mutex);
	pthread_mutex_unlock(&dongles->large_dongle->mutex);
	if (!init_request(dongles, coder, ticket, deadline))
		return (0);
	return (1);
}
