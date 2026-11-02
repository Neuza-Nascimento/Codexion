/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 18:27:52 by nedo-nas          #+#    #+#             */
/*   Updated: 2026/11/02 18:27:55 by nedo-nas         ###   ########.fr       */
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
	return ((long)((temp.tv_sec * 1000) + (temp.tv_usec / 1000)));
}

struct timespec	future_time(long time_in_ms)
{
	struct timespec	abstime;
	long			future_time_ms;

	future_time_ms = current_time_ms() + time_in_ms;
	abstime.tv_sec = future_time_ms / 1000;
	abstime.tv_nsec = (future_time_ms % 1000) * 1000000;
	return (abstime);
}

void	log_state(int id, char *message, t_table *table)
{
	long	time;

	pthread_mutex_lock(&table->log_mutex);
	time = current_time_ms() - table->start_time;
	printf("%ld %i %s\n", time, id, message);
	pthread_mutex_unlock(&table->log_mutex);
}
