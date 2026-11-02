/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 16:29:50 by nedo-nas          #+#    #+#             */
/*   Updated: 2026/11/02 18:28:05 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_compiling(t_coder *coder)
{
	t_table		*table;
	t_dongles	dongles;

	table = coder->table;
	dongles.small_dongle = smallest_dongle(coder->dongle_left,
			coder->dongle_rigth);
	dongles.large_dongle = large_dongle(coder->dongle_left,
			coder->dongle_rigth);
	dongles.remove = 0;
	if (coder->dongle_left == coder->dongle_rigth)
		return (log_state(coder->id, "has taken a dongle", table), 0);
	if (!acquire_dongles(&dongles, coder))
		return (0);
	pthread_mutex_lock(&coder->status_coder);
	coder->last_compile_start = current_time_ms();
	pthread_mutex_unlock(&coder->status_coder);
	log_state(coder->id, "is compiling", table);
	usleep(table->time_compile * 1000);
	leave_dongle(dongles.small_dongle, table->dongle_cooldown);
	leave_dongle(dongles.large_dongle, table->dongle_cooldown);
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

	coder = (t_coder *)arg;
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
