/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   helpers.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/22 15:59:09 by username         #+#    #+#              */
/*   Updated: 2026/09/29 14:12:20 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

t_dongle	*large_dongle(t_dongle *d1, t_dongle *d2)
{
	if (d1->id < d2->id)
		return (d2);
	return (d1);
}

t_dongle	*smallest_dongle(t_dongle *d1, t_dongle *d2)
{
	if (d1->id > d2->id)
		return (d2);
	return (d1);
}

long	current_time_ms(void)
{
	struct timeval	temp;

	gettimeofday(&temp, NULL);
	return (long)((temp.tv_sec * 1000) + (temp.tv_usec / 1000));
}

void	log_state(int id, char *message, t_table *table)
{
	long	time;

	pthread_mutex_lock(&table->log_mutex);
	time = current_time_ms() - table->start_time;
	printf("%ld %i %s\n", time, id, message);
	pthread_mutex_unlock(&table->log_mutex);
}

int	simulation_over(t_table *table)
{
	int	value;

	pthread_mutex_lock(&table->stop_mutex);
	value = table->simulation_finish;
	pthread_mutex_unlock(&table->stop_mutex);
	return (value);
}

void	print_table(t_table *table)
{
	int	i;

	if (!table)
	{
		printf("TABLE: NULL\n");
		return ;
	}
	printf("========== TABLE ==========\n");
	printf("number_coders:    %d\n", table->number_coders);
	printf("time_burnout:     %d\n", table->time_burnout);
	printf("time_compile:     %d\n", table->time_compile);
	printf("time_debug:       %d\n", table->time_debug);
	printf("time_refactor:    %d\n", table->time_refactor);
	printf("compiles_required: %d\n", table->compiles_required);
	printf("dongle_cooldown:  %d\n", table->dongle_cooldown);
	printf("time_start:       %ld\n", table->start_time);
	printf("scheduler:        %d\n", table->scheduler);
	printf("\n---------- DONGLES ----------\n");
	i = 0;
	while (i < table->number_coders)
	{
		printf("Dongle [%d]\n", i);
		printf("  id:      %d\n", table->dongles[i].id);
		printf("  is_busy: %d\n", table->dongles[i].is_busy);
		i++;
	}
	printf("\n----------- CODERS -----------\n");
	i = 0;
	while (i < table->number_coders)
	{
		printf("Coder [%d]\n", i);
		printf("  id:                  %d\n", table->coders[i].id);
		printf("  status:              %d\n", table->coders[i].status);
		printf("  last_compile_start:  %ld\n",
			table->coders[i].last_compile_start);
		printf("  dongle_left:         %d\n", table->coders[i].dongle_left->id);
		printf("  dongle_right:        %d\n",
			table->coders[i].dongle_rigth->id);
		printf("  holds_left:          %d\n", table->coders[i].holds_left);
		printf("  holds_right:         %d\n", table->coders[i].holds_rigth);
		printf("  compiles_done:       %d\n", table->coders[i].compiles_done);
		i++;
	}
	printf("=============================\n");
}
