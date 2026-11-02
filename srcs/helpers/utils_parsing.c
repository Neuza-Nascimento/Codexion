/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 12:30:52 by nedo-nas          #+#    #+#             */
/*   Updated: 2026/11/02 18:27:45 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

int	is_integer(long number)
{
	if (number > 2147483647 || number < -2147483648)
		return (0);
	return (1);
}

int	is_valid_number(char *av)
{
	size_t	i;

	i = 0;
	if (av[i] == '-' || av[i] == '+')
		i++;
	if (!av[i] || av[0] == '\0')
		return (0);
	while (av[i])
	{
		if (av[i] < '0' || av[i] > '9')
			return (0);
		i++;
	}
	return (1);
}
