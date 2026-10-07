/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 16:29:50 by username          #+#    #+#             */
/*   Updated: 2026/10/07 15:14:04 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	leave_dongle(t_dongle *dongle, long cooldown)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_busy = 0;
	dongle->available_at = current_time_ms() + cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);

}

void verify_await(t_dongle *dongle, t_coder *coder)
{
	long time;
	struct timespec time_f;
	

	if (simulation_over(coder->table))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return ;
	}
	else if (coder->id != dongle->requests[0].id_coder || dongle->is_busy == 1)
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	else if (coder->id == dongle->requests[0].id_coder && dongle->is_busy == 0 && current_time_ms() < dongle->available_at)
	{
		time = dongle->available_at - current_time_ms();
		time_f = future_time(time);
		pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &time_f);
	}
	pthread_mutex_unlock(&dongle->mutex);
}

int	acquire_dongles(t_dongle *dongle1, t_dongle *dongle2, t_coder *coder)
{
	while (1)
	{
		pthread_mutex_lock(&dongle1->mutex);
		pthread_mutex_lock(&dongle2->mutex);
		if (simulation_over(coder->table))
		{
			pthread_mutex_unlock(&dongle1->mutex);
			pthread_mutex_unlock(&dongle2->mutex);
			return (0);
		}
		if (dongle_ok(dongle1, coder) && dongle_ok(dongle2, coder))
		{
			remove_top(dongle2, coder->table);
			remove_top(dongle1, coder->table);
			dongle1->is_busy = 1;
			dongle2->is_busy = 1;
			log_state(coder->id, "has taken a dongle", coder->table);
			log_state(coder->id, "has taken a dongle", coder->table);
			break;
		}
		else 
		{
			if (!dongle_ok(dongle1, coder))
			{
				pthread_mutex_unlock(&dongle2->mutex);
				verify_await(dongle1, coder);
			}
			else
			{
				pthread_mutex_unlock(&dongle1->mutex);
				verify_await(dongle2, coder);	
			}		
		}
	}
	pthread_mutex_unlock(&dongle1->mutex);
	pthread_mutex_unlock(&dongle2->mutex);
	return (1);
}

int	is_compiling(t_coder *coder)
{
	t_table  *table;
	t_dongle *small_dongl;
	t_dongle *large_dongl;

	table = coder->table;
	small_dongl = smallest_dongle(coder->dongle_left, coder->dongle_rigth);
	large_dongl = large_dongle(coder->dongle_left, coder->dongle_rigth);
	if (coder->dongle_left == coder->dongle_rigth)
	{
		log_state(coder->id, "has taken a dongle", table);
		return (0);
	}
	create_request(small_dongl, coder);
	create_request(large_dongl, coder);
	if (!acquire_dongles(small_dongl, large_dongl, coder))
		return (0);
	pthread_mutex_lock(&coder->status_coder);
	coder->last_compile_start = current_time_ms();
	pthread_mutex_unlock(&coder->status_coder);
	log_state(coder->id, "is compiling", table);
	usleep(table->time_compile * 1000);
	leave_dongle(small_dongl, table->dongle_cooldown);
	leave_dongle(large_dongl, table->dongle_cooldown);
	pthread_mutex_lock(&coder->status_coder);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->status_coder);
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
