/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 13:31:12 by nedo-nas          #+#    #+#             */
/*   Updated: 2026/09/28 13:31:12 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	display_error(void)
{
	fprintf(stderr, "ERROR: invalid arguments \n");
	return (1);
}

int	main(int ac, char **av)
{
	t_table	table;

	if (ac != 9)
		return (display_error());
	if (!parsing(&table, av + 1))
		return (display_error());
	if (!iniciate_table(&table))
		return (free(table.coders), free(table.dongles), display_error());
	if (!simulation(&table))
		return (free(table.coders), free(table.dongles), display_error());
	destroy_mutexes(&table);
	return (free(table.coders), free(table.dongles), 0);
}
