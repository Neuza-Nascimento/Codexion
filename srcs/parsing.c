/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 12:00:37 by nedo-nas          #+#    #+#             */
/*   Updated: 2026/09/25 12:25:10 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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

int	ft_atol(char *str)
{
	int		i;
	long	res;
	long	sign;

	i = 0;
	res = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

int	starts_data(char **av, t_table *data)
{
	data->number_coders = atoi(av[0]);
	data->time_burnout = atoi(av[1]);
	data->time_compile = atoi(av[2]);
	data->time_debug = atoi(av[3]);
	data->time_refactor = atoi(av[4]);
	data->compiles_required = atoi(av[5]);
	data->dongle_cooldown = atoi(av[6]);
	data->start_time = current_time_ms();
	if (strcmp(av[7], "fifo") == 0)
		data->scheduler = FIFO;
	else if (strcmp(av[7], "edf") == 0)
		data->scheduler = EDF;
	else
		return (0);
	return (1);
}

t_table	*parsing(char **av)
{
	t_table	*data;
	int				count;
	long			value;

	count = 0;
	while (count < 7)
	{
		if (is_valid_number(av[count]))
		{
			value = atol(av[count]);
			if (value < 0 || !is_integer(value))
				return (NULL);
			count++;
		}
		else
			return (NULL);
	}
	data = malloc(sizeof(t_table));
	if (!data)
		return (NULL);
	if (!starts_data(av, data))
		return (free(data), NULL);
	return (data);
}
