/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 11:03:58 by nedo-nas          #+#    #+#             */
/*   Updated: 2026/09/22 16:54:16 by nedo-nas         ###   ########.fr       */
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
	t_initial_data *data;

	if (ac != 9)
		return (display_error());
	data = parsing(av + 1);
	if (!data)
		return (display_error());
	return (free(data), 0);
}