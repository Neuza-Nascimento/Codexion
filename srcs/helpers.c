/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 15:59:09 by nedo-nas          #+#    #+#             */
/*   Updated: 2026/09/22 16:44:06 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void display_initial_data(t_initial_data* data) {
	printf("========== INITIAL DATA ==========\n");
	printf("Number coders      : %d\n", data->number_coders);
	printf("Time burnout       : %d\n", data->time_burnout);
	printf("Time compile       : %d\n", data->time_compile);
	printf("Time debug         : %d\n", data->time_debug);
	printf("Time refactor      : %d\n", data->time_refactor);
	printf("Compiles required  : %d\n", data->compiles_required);
	printf("Dongle cooldown    : %d\n", data->dongle_cooldown);

	printf("Scheduler           : ");
	if (data->scheduler == FIFO)
		printf("FIFO\n");
	else if (data->scheduler == EDF)
		printf("EDF\n");	
	printf("==================================\n");
}

