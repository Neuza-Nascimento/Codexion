/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 16:29:50 by username          #+#    #+#             */
/*   Updated: 2026/10/01 11:11:22 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	leave_dongle(t_dongle *dongle, long cooldown)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_busy = 0;
	dongle->available_at = current_time_ms() + cooldown;
	pthread_cond_signal(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);

}

int	acquire_dongle(t_dongle *dongle, t_coder *coder)
{
	long		last_compile;
	t_request	request;

	pthread_mutex_lock(&coder->status_coder);
	request.id_coder = coder->id;
	last_compile = coder->last_compile_start;
	pthread_mutex_unlock(&coder->status_coder);
	request.dealine = last_compile + coder->table->time_burnout;
	pthread_mutex_lock(&dongle->mutex);
	request.ticket = dongle->count_tickets;
	dongle->count_tickets++;
	push_request(dongle, &request, coder->table);
	while (1)
	{
		if (simulation_over(coder->table))
		{
			remove_top(dongle, coder->table);
			pthread_mutex_unlock(&dongle->mutex);
			return (0);
		}
		else if (dongle->requests[0]->id_coder != coder->id || dongle->is_busy == 1)
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
		else if (dongle->requests[0]->id_coder != coder->id && dongle->is_busy == 0 && current_time_ms() < dongle->available_at)
			usleep(2000);
		else
			break ;
	}
	remove_top(dongle, coder->table);
	dongle->is_busy = 1;
	pthread_mutex_unlock(&dongle->mutex);
	return (1);
}

int	is_compiling(t_coder *coder)
{
	t_dongle	*large_dongl;
	t_dongle	*small_dongl;
	t_table		*table;

	table = coder->table;
	large_dongl = large_dongle(coder->dongle_left, coder->dongle_rigth);
	small_dongl = smallest_dongle(coder->dongle_left, coder->dongle_rigth);
	if (!acquire_dongle(small_dongl, coder))
		return (0);
	log_state(coder->id, "has taken a dongle", table);
	if (coder->dongle_left == coder->dongle_rigth)
		return (0);
	if (!acquire_dongle(large_dongl, coder))
		return (leave_dongle(small_dongl, table->dongle_cooldown), 0);
	log_state(coder->id, "has taken a dongle", table);
	pthread_mutex_lock(&coder->status_coder);
	coder->last_compile_start = current_time_ms();
	pthread_mutex_unlock(&coder->status_coder);
	log_state(coder->id, "is compiling", table);
	usleep(table->time_compile * 1000);
	pthread_mutex_lock(&coder->status_coder);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->status_coder);
	leave_dongle(small_dongl, table->dongle_cooldown);
	leave_dongle(large_dongl, table->dongle_cooldown);
	return (1);
}

void	is_debugging(int id, t_table *table)
{
	log_state(id, "is debugging", table);
	usleep(table->time_debug * 1000);
}

void	is_refactoring(int id, t_table *table)
{
	log_state(id, "is refactoring", table);
	usleep(table->time_refactor * 1000);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_table	*table;

	coder = (t_coder *) arg;
	table = coder->table;
	while (!simulation_over(table))
	{
		if (!is_compiling(coder))
			return (NULL);
		if (!simulation_over(table))	
			is_debugging(coder->id, table);
		if (!simulation_over(table))
			is_refactoring(coder->id, table);
	}
	return (NULL);
}
