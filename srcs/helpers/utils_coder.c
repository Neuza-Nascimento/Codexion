/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_coder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/08 11:45:31 by username          #+#    #+#             */
/*   Updated: 2026/10/08 13:07:56 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	dongle_ok(t_dongle *dongle, t_coder *coder)
{
	return (dongle->requests[0].id_coder == coder->id && dongle->is_busy == 0 && current_time_ms() >= dongle->available_at);
}
void verify_wait(t_dongle *dongle, t_coder *coder)
{
	long time;
	struct timespec time_f;
	

	if (simulation_over(coder->table))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return ;
	}
	else if (coder->id != dongle->requests[0].id_coder || dongle->is_busy == 1)
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	else if (coder->id == dongle->requests[0].id_coder && dongle->is_busy == 0 && current_time_ms() < dongle->available_at)
	{
		time = dongle->available_at - current_time_ms();
		time_f = future_time(time);
		pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &time_f);
	}
	pthread_mutex_unlock(&dongle->mutex);
}

void wait_dongles(t_dongle *dongle1, t_dongle *dongle2, t_coder *coder)
{
	if (!dongle_ok(dongle1, coder))
	{
		pthread_mutex_unlock(&dongle2->mutex);
		verify_wait(dongle1, coder);
	}
	else
	{
		pthread_mutex_unlock(&dongle1->mutex);
		verify_wait(dongle2, coder);	
	}	
}

void take_dongles(t_dongle *dongle1, t_dongle *dongle2, t_coder *coder)
{
	remove_top(dongle1, coder->table);
	remove_top(dongle2, coder->table);
	dongle1->is_busy = 1;
	dongle2->is_busy = 1;
	log_state(coder->id, "has taken a dongle", coder->table);
	log_state(coder->id, "has taken a dongle", coder->table);
}

void	leave_dongle(t_dongle *dongle, long cooldown)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_busy = 0;
	dongle->available_at = current_time_ms() + cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);

}

int	acquire_dongles(t_dongle *dongle1, t_dongle *dongle2, t_coder *coder)
{
	pthread_mutex_lock(&dongle1->mutex);
	pthread_mutex_lock(&dongle2->mutex);
	iniciate_request(coder, coder->table, dongle1, dongle2);
	pthread_mutex_unlock(&dongle1->mutex);
	pthread_mutex_unlock(&dongle2->mutex);
	while (1)
	{
		pthread_mutex_lock(&dongle1->mutex);
		pthread_mutex_lock(&dongle2->mutex);
		if (simulation_over(coder->table))
		{
			pthread_mutex_unlock(&dongle1->mutex);
			pthread_mutex_unlock(&dongle2->mutex);
			return (0);
		}
		if (dongle_ok(dongle1, coder) && dongle_ok(dongle2, coder))
		{
			take_dongles(dongle1, dongle2, coder);
			break;
		}
		else 
			wait_dongles(dongle1, dongle2, coder);
	}
	pthread_mutex_unlock(&dongle1->mutex);
	pthread_mutex_unlock(&dongle2->mutex);
	return (1);
}