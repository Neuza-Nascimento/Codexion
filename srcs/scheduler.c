/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/09 14:15:10 by nedo-nas          #+#    #+#             */
/*   Updated: 2026/10/09 14:29:45 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	create_request(t_dongle *dongle, t_coder *coder, t_request *request)
{
	push_request(dongle, request, coder->table);
}

int	dongle_ok(t_dongle *dongle, t_coder *coder)
{
	return (dongle->requests[0].id_coder == coder->id && dongle->is_busy == 0 
            && current_time_ms() >= dongle->available_at);
}

void iniciate_request(t_coder *coder, int ticket, long deadline, int remove, t_dongle *dongle1, t_dongle *dongle2)
{
	t_request request;

	pthread_mutex_lock(&coder->status_coder);
	request.id_coder = coder->id;
	pthread_mutex_unlock(&coder->status_coder);
	request.dealine = deadline;
	request.ticket = ticket;
	if (remove == 1)
		create_request(dongle1, coder, &request);
	else if (remove == 2)
		create_request(dongle2, coder, &request);
	else
	{
		create_request(dongle1, coder, &request);
		create_request(dongle2, coder, &request);
	}
}
