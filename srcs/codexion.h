/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 11:04:41 by nedo-nas          #+#    #+#             */
/*   Updated: 2026/09/23 11:58:08 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

enum					e_scheduler
{
	EDF,
	FIFO,
};

enum					e_state
{
	TAKING_DONGLE,
	COMPILING,
	DEBUGGING,
	REFACTORING,
	DEAD,
};

typedef struct s_dongle
{
	int					id;
	int					is_busy;
}						t_dongle;

typedef struct s_coder
{
	int					id;
	enum e_state		status;
	int					last_compile_start;
	t_dongle			*dongle_left;
	t_dongle			*dongle_rigth;
	int					holds_left;
	int					holds_rigth;
	int					compiles_done;
	pthread_t			thread;

}						t_coder;

typedef struct s_table
{
	int					number_coders;
	int					time_burnout;
	int					time_compile;
	int					time_debug;
	int					time_refactor;
	int					compiles_required;
	int					dongle_cooldown;
	enum e_scheduler	scheduler;
	t_dongle			*dongles;
	t_coder				*coders;

}						t_table;

t_table					*parsing(char **av);
int	iniciate_table(t_table *table);
void print_table(t_table *data);

#endif
