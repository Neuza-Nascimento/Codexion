/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_dongle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 16:36:23 by nedo-nas          #+#    #+#             */
/*   Updated: 2026/11/02 16:38:35 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	wait_dongle(t_dongle *dongle, t_coder *coder)
{
	long			time;
	struct timespec	time_f;

	if (simulation_over(coder->table))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return ;
	}
	else if (coder->id != dongle->requests[0].id_coder || dongle->is_busy == 1)
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	else if (coder->id == dongle->requests[0].id_coder && dongle->is_busy == 0
		&& current_time_ms() < dongle->available_at)
	{
		time = dongle->available_at - current_time_ms();
		time_f = future_time(time);
		pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &time_f);
	}
	pthread_mutex_unlock(&dongle->mutex);
}

int	init_request(t_dongles *dongles, t_coder *coder, int ticket, long deadline)
{
	while (1)
	{
		pthread_mutex_lock(&dongles->small_dongle->mutex);
		pthread_mutex_lock(&dongles->large_dongle->mutex);
		if (simulation_over(coder->table))
		{
			pthread_mutex_unlock(&dongles->small_dongle->mutex);
			pthread_mutex_unlock(&dongles->large_dongle->mutex);
			return (0);
		}
		if (dongles->remove == 1 || dongles->remove == 2)
			iniciate_request(coder, ticket, deadline, dongles);
		if (dongle_ok(dongles->small_dongle, coder)
			&& dongle_ok(dongles->large_dongle, coder))
		{
			take_dongles(dongles, coder);
			break ;
		}
		else
			dongles->remove = wait_dongles(dongles, coder);
	}
	pthread_mutex_unlock(&dongles->small_dongle->mutex);
	pthread_mutex_unlock(&dongles->large_dongle->mutex);
	return (1);
}
