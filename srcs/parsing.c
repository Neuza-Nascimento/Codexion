/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   parsing.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username<nedo-nas@student.42porto.com>   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/14 12:00:37 by username         #+#    #+#              */
/*   Updated: 2026/09/29 14:12:57 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	starts_table(char **av, t_table *table)
{
	table->number_coders = atoi(av[0]);
	if (table->number_coders <= 0)
		return (0);
	table->time_burnout = atoi(av[1]);
	table->time_compile = atoi(av[2]);
	table->time_debug = atoi(av[3]);
	table->time_refactor = atoi(av[4]);
	table->compiles_required = atoi(av[5]);
	table->dongle_cooldown = atoi(av[6]);
	table->start_time = 0;
	table->simulation_finish = 0;
	table->count_tickets = 0;
	pthread_mutex_init(&table->stop_mutex, NULL);
	pthread_mutex_init(&table->log_mutex, NULL);
	if (strcmp(av[7], "fifo") == 0)
		table->scheduler = FIFO;
	else if (strcmp(av[7], "edf") == 0)
		table->scheduler = EDF;
	else
		return (0);
	return (1);
}

int	parsing(t_table *table, char **av)
{
	int		count;
	long	value;

	count = 0;
	while (count < 7)
	{
		if (is_valid_number(av[count]))
		{
			value = atol(av[count]);
			if (value < 0 || !is_integer(value))
				return (0);
			count++;
		}
		else
			return (0);
	}
	if (!starts_table(av, table))
		return (0);
	return (1);
}
