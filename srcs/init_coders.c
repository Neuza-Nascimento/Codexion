/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 10:38:31 by nedo-nas          #+#    #+#             */
/*   Updated: 2026/09/23 12:02:23 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	iniciate_dongles(t_table *table)
{
	int	i;

	table->dongles = malloc(sizeof(t_dongle) * table->number_coders);
	if (!table->dongles)
		return (0);
	i = 0;
	while (i < table->number_coders)
	{
		table->dongles[i].id = i + 1;
		table->dongles[i].is_busy = 0;
		i++;
	}
	return (1);
}

int	iniciate_coders(t_table *table)
{
	int	i;

	i = 0;
	table->coders = malloc(sizeof(t_coder) * table->number_coders);
	if (!table->coders)
		return (0);
	while (i < table->number_coders)
	{
		table->coders[i].compiles_done = 0;
		table->coders[i].dongle_left = &table->dongles[i];
		table->coders[i].dongle_rigth = &table->dongles[(i + 1) % table->number_coders] ;
		table->coders[i].holds_left = 0;
		table->coders[i].holds_rigth = 0;
		table->coders[i].id = i + 1;
		table->coders[i].last_compile_start = 0;
		table->coders[i].status = TAKING_DONGLE;
		i++;
	}
	return (1);
}

int	iniciate_table(t_table *table)
{
	if (!iniciate_dongles(table) || !iniciate_coders(table))
		return (0);
	return (1);
}