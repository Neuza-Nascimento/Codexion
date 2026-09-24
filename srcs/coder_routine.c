/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 14:53:32 by nedo-nas          #+#    #+#             */
/*   Updated: 2026/09/24 18:04:31 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	simulation_over(t_table *table)
{
	int	value;

	pthread_mutex_lock(&table->stop_mutex);
	value = table->simulation_over;
	pthread_mutex_unlock(&table->stop_mutex);
	return (value);
}

void	*coder_routine(void *arg)
{
	t_coder *coder;
	t_table *table;
	
	table = (t_table *)arg;
	coder = &table->coders[table->current_coder];

	while (!simulation_over((t_table *)arg))
	{
		pthread_mutex_lock(&coder->dongle_left->mutex);
		log_state(coder->id, "has taken a dongle", table);
		pthread_mutex_lock(&coder->dongle_rigth->mutex);
		log_state(coder->id, "has taken a dongle", table);
		log_state(coder->id, "is compiling", table);
		usleep(table->time_compile * 1000);
		pthread_mutex_unlock(&coder->dongle_left->mutex);
		pthread_mutex_unlock(&coder->dongle_rigth->mutex);
		log_state(coder->id, "is debugging", table);
		usleep(table->time_debug * 1000);
		log_state(coder->id, "is refactoring", table);
		usleep(table->time_refactor * 1000);
	}

	return NULL;
}

void	iniciate_simulation(t_table *table)
{
	int i;

	i = 0;
	while (i < table->number_coders)
	{
		table->current_coder = i;
		pthread_create(&table->coders[i].thread, NULL, coder_routine,
			(void *)table);
		i++;
	}
	i = 0;
	while (i < table->number_coders)
	{
		pthread_join(table->coders[i].thread, NULL);
		i++;
	}
}