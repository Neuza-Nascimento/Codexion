/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_scheduler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 13:16:47 by username          #+#    #+#             */
/*   Updated: 2026/10/07 14:47:18 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	dongle_ok(t_dongle *dongle, t_coder *coder)
{
	return (dongle->requests[0].id_coder == coder->id && dongle->is_busy == 0 && current_time_ms() >= dongle->available_at);
}
