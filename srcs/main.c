/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 18:28:41 by nedo-nas          #+#    #+#             */
/*   Updated: 2026/11/04 09:58:28 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	display_error(char *str)
{
	fprintf(stderr, "%s", str);
	return (1);
}

int	main(int ac, char **av)
{
	t_table	table;

	if (ac != 9)
		return (display_error("ERROR: invalid arguments \n"));
	if (!parsing(&table, av + 1))
		return (display_error("ERROR: invalid arguments \n"));
	if (!iniciate_table(&table))
		return (free(table.coders), free(table.dongles),
			display_error("ERROR : error initializing the table\n"));
	if (!simulation(&table))
		return (free(table.coders), free(table.dongles),
			display_error("ERROR : error creating threads\n"));
	destroy_mutexes(&table);
	return (free(table.coders), free(table.dongles), 0);
}
