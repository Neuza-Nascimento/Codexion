/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gettime.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 14:13:19 by nedo-nas          #+#    #+#             */
/*   Updated: 2026/09/24 14:25:42 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

long	current_time_ms(void)
{
	struct timeval	temp;

	gettimeofday(&temp, NULL);
	return (long)(temp.tv_sec * 1000 + temp.tv_sec / 1000);
}

long	get_timestamp_ms(long start_time)
{
	return (current_time_ms() - start_time);
}