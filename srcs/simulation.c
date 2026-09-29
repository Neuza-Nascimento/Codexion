/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   simulation.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/28 18:12:44 by username         #+#    #+#              */
/*   Updated: 2026/09/29 14:13:02 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	init_time(t_table *table)
{
	int	i;

	i = 0;
	table->start_time = current_time_ms();
	while (i < table->number_coders)
	{
		table->coders[i].last_compile_start = table->start_time;
		i++;
	}
}

static int	create_threads(t_table *table)
{
	int	i;

	if (pthread_create(&table->monitor, NULL, monitor_routine, table) != 0)
		return (0);
	i = 0;
	while (i < table->number_coders)
	{
		table->coders[i].table = table;
		if (pthread_create(&table->coders[i].thread, NULL, coder_routine,
					&table->coders[i]) != 0)
		return (0);
		i++;
	}
	return (1);
}

static void	join_threads(t_table *table)
{
	int	i;

	i = 0;
	pthread_join(table->monitor, NULL);
	while (i < table->number_coders)
	{
		pthread_join(table->coders[i].thread, NULL);
		i++;
	}
}

int	simulation(t_table *table)
{
	init_time(table);
	if (!create_threads(table))
		return (0);
	join_threads(table);
	return (1);
}
