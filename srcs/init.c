/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   init.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/23 10:38:31 by username         #+#    #+#              */
/*   Updated: 2026/09/29 14:12:34 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	iniciate_dongles(t_table *table)
{
	int	i;

	table->dongles = malloc(sizeof(t_dongle)*table->number_coders);
	if (!table->dongles)
		return (0);
	i = 0;
	while (i < table->number_coders)
	{
		table->dongles[i].id = i + 1;
		table->dongles[i].is_busy = 0;
		table->dongles[i].available_at = current_time_ms();
		table->dongles[i].len_requests = 0;
		table->dongles[i].count_tickets = 0;
   		table->dongles[i].requests[0] = NULL;
		table->dongles[i].requests[1] = NULL;
		pthread_mutex_init(&table->dongles[i].mutex, NULL);
		pthread_cond_init(&table->dongles[i].cond, NULL);
		i++;
	}
	return (1);
}

int	iniciate_coders(t_table *table)
{
	int	i;

	i = 0;
	table->coders = malloc(sizeof(t_coder)*table->number_coders);
	if (!table->coders)
		return (0);
	while (i < table->number_coders)
	{
		table->coders[i].compiles_done = 0;
		table->coders[i].dongle_left = &table->dongles[i];
		table->coders[i].dongle_rigth = &table->dongles[(i + 1) % table->number_coders];
		table->coders[i].holds_left = 0;
		table->coders[i].holds_rigth = 0;
		table->coders[i].id = i + 1;
		table->coders[i].last_compile_start = 0;
		table->coders[i].status = TAKING_DONGLE;
		pthread_mutex_init(&table->coders[i].status_coder, NULL);
		i++;
	}
	return (1);
}

void	destroy_mutexes(t_table *table)
{
	int	i;

	pthread_mutex_destroy(&table->log_mutex);
	pthread_mutex_destroy(&table->stop_mutex);
	i = 0;
	while (i < table->number_coders)
	{
		pthread_mutex_destroy(&table->dongles[i].mutex);
		pthread_mutex_destroy(&table->coders[i].status_coder);
		i++;
	}
}

int	iniciate_table(t_table *table)
{
	if (!iniciate_dongles(table) || !iniciate_coders(table))
		return (0);
	return (1);
}
