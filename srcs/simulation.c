/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 14:53:32 by nedo-nas          #+#    #+#             */
/*   Updated: 2026/09/25 14:45:40 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_state(int id, char *message, t_table *table)
{
	long	time;

	pthread_mutex_lock(&table->log_mutex);
	time = current_time_ms() - table->start_time;
	printf(" %ld %i %s\n", time, id, message);
	pthread_mutex_unlock(&table->log_mutex);
}

int	simulation_over(t_table *table)
{
	int	value;

	pthread_mutex_lock(&table->stop_mutex);
	value = table->simulation_over;
	pthread_mutex_unlock(&table->stop_mutex);
	return (value);
}

void	is_compiling(int id, t_table *table)
{
	log_state(id, "is compiling", table);
	usleep(table->time_compile * 1000);
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
	t_coder		*coder;
	t_table		*table;
	t_dongle	*large_dongl;
	t_dongle	*small_dongl;

	coder = (t_coder *)arg;
	table = coder->table;
	while (!simulation_over(table))
	{
		large_dongl = large_dongle(coder->dongle_left, coder->dongle_rigth);
		small_dongl = smallest_dongle(coder->dongle_left, coder->dongle_rigth);
		pthread_mutex_lock(&small_dongl->mutex);
		log_state(coder->id, "has taken a dongle", table);
		pthread_mutex_lock(&large_dongl->mutex);
		log_state(coder->id, "has taken a dongle", table);
		is_compiling(coder->id, table);
		coder->compiles_done++;
		pthread_mutex_unlock(&coder->dongle_left->mutex);
		pthread_mutex_unlock(&coder->dongle_rigth->mutex);
		is_debugging(coder->id, table);
		is_refactoring(coder->id, table);
	}
	return (NULL);
}

void	simulation(t_table *table)
{
	int i;

	i = 0;
	while (i < table->number_coders)
	{
		table->coders[i].table = table;
		pthread_create(&table->coders[i].thread, NULL, coder_routine,
			(void *)&table->coders[i]);
		i++;
	}
	i = 0;
	while (i < table->number_coders)
	{
		pthread_join(table->coders[i].thread, NULL);
		i++;
	}
}