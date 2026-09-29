/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   parsing.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/14 12:00:37 by username         #+#    #+#              */
/*   Updated: 2026/09/29 14:12:57 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	starts_data(char **av, t_table *data)
{
	data->number_coders = atoi(av[0]);
	data->time_burnout = atoi(av[1]);
	data->time_compile = atoi(av[2]);
	data->time_debug = atoi(av[3]);
	data->time_refactor = atoi(av[4]);
	data->compiles_required = atoi(av[5]);
	data->dongle_cooldown = atoi(av[6]);
	data->start_time = 0;
	data->simulation_finish = 0;
	pthread_mutex_init(&data->stop_mutex, NULL);
	pthread_mutex_init(&data->log_mutex, NULL);
	if (strcmp(av[7], "fifo") == 0)
		data->scheduler = FIFO;
	else if (strcmp(av[7], "edf") == 0)
		data->scheduler = EDF;
	else
		return (0);
	return (1);
}

int	parsing(t_table *data, char **av)
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
	if (!starts_data(av, data))
		return (0);
	return (1);
}
