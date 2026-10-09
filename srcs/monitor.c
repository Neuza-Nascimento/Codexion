/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 10:55:29 by username          #+#    #+#             */
/*   Updated: 2026/10/09 14:32:32 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	simulation_over(t_table *table)
{
	int	value;

	pthread_mutex_lock(&table->stop_mutex);
	value = table->simulation_finish;
	pthread_mutex_unlock(&table->stop_mutex);
	return (value);
}

void	stop_simulation(t_table *table)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&table->stop_mutex);
	table->simulation_finish = 1;
	pthread_mutex_unlock(&table->stop_mutex);
	while (i < table->number_coders)
	{
		pthread_mutex_lock(&table->dongles[i].mutex);
		pthread_cond_broadcast(&table->dongles[i].cond);
		pthread_mutex_unlock(&table->dongles[i].mutex);
		i++;
	}
}

static int	check_burnout(t_table *table)
{
	int		i;
	long	last_compile;

	i = 0;
	while (i < table->number_coders)
	{
		pthread_mutex_lock(&table->coders[i].status_coder);
		last_compile = table->coders[i].last_compile_start;
		pthread_mutex_unlock(&table->coders[i].status_coder);
		if (current_time_ms() - last_compile >= table->time_burnout)
		{
			log_state(table->coders[i].id, "burned out", table);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	check_compiles(t_table *table)
{
	int	i;
	int	compile_count;

	i = 0;
	while (i < table->number_coders)
	{
		pthread_mutex_lock(&table->coders[i].status_coder);
		compile_count = table->coders[i].compiles_done;
		pthread_mutex_unlock(&table->coders[i].status_coder);
		if (compile_count < table->compiles_required)
			return (0);
		i++;
	}
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_table	*table;

	table = (t_table *) arg;
	while (!simulation_over(table))
	{
		if (check_burnout(table))
		{
			stop_simulation(table);
			return (NULL);
		}
		if (check_compiles(table))
		{
			stop_simulation(table);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
