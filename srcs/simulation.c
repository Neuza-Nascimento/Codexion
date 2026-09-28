/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 18:12:44 by nedo-nas          #+#    #+#             */
/*   Updated: 2026/09/28 18:12:44 by nedo-nas         ###   ########.fr       */
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

int		try_take_dongle(t_dongle *dongle)
{
	int sucess;

	sucess = 0;
	pthread_mutex_lock(&dongle->mutex);
	if (dongle->is_busy == 0 && current_time_ms() >= dongle->available_at)
	{
		dongle->is_busy = 1;
		sucess = 1;
	}
	pthread_mutex_unlock(&dongle->mutex);
	return (sucess);
}

void leave_dongle(t_dongle *dongle, long cooldown)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_busy = 0;
	dongle->available_at = current_time_ms() + cooldown; 
	pthread_mutex_unlock(&dongle->mutex);

}

int acquire_dongle(t_dongle *dongle, t_table *table)
{
    while (1)
    {
        if (simulation_over(table))
			return (0);
		if (try_take_dongle(dongle))
			return (1);
    	usleep(100);
	}
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

	coder = (t_coder *) arg;
	table = coder->table;
	while (!simulation_over(table))
	{
		large_dongl = large_dongle(coder->dongle_left, coder->dongle_rigth);
		small_dongl = smallest_dongle(coder->dongle_left, coder->dongle_rigth);
		if (!acquire_dongle(small_dongl, table))
			return (NULL);
		log_state(coder->id, "has taken a dongle", table);
		if (!acquire_dongle(large_dongl, table))
			return (leave_dongle(small_dongl, table->dongle_cooldown), NULL);
		log_state(coder->id, "has taken a dongle", table);
		is_compiling(coder->id, table);
		coder->compiles_done++;
		leave_dongle(small_dongl, table->dongle_cooldown);
		leave_dongle(large_dongl, table->dongle_cooldown);
		is_debugging(coder->id, table);
		is_refactoring(coder->id, table);

	}
	return (NULL);
}

int	simulation(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->number_coders)
	{
		table->coders[i].table = table;
		if (pthread_create(&table->coders[i].thread, NULL, coder_routine,
				&table->coders[i]) != 0)
			return (0);
		i++;
	}
	i = 0;
	while (i < table->number_coders)
	{
		pthread_join(table->coders[i].thread, NULL);
		i++;
	}
	return (1);
}
