/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 11:03:58 by nedo-nas          #+#    #+#             */
/*   Updated: 2026/09/23 12:13:34 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	display_error(void)
{
	fprintf(stderr, "ERROR: invalid arguments \n");
	fprintf(stderr,
		"Usage: number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler\n");
	return (1);
}

int	main(int ac, char **av)
{
	t_table *table;

	if (ac != 9)
		return (display_error());
	table = parsing(av + 1);
	if (!table)
		return (display_error());
	if (!iniciate_table(table))
		return (free(table->coders), free(table->dongles),
			free(table),display_error());
	print_table(table);
	printf("Deu Certo");
	return (free(table->coders), free(table->dongles), free(table), 0);
}