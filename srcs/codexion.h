/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nedo-nas <nedo-nas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 11:04:41 by nedo-nas          #+#    #+#             */
/*   Updated: 2026/09/22 17:14:53 by nedo-nas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
#define CODEXION_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum e_scheduler {
	EDF,
	FIFO,
};

enum e_state {
	TAKING_DONGLE,
	COMPILING,
	DEBUGGING,
	REFACTORING,
	DEAD,
};

typedef struct s_initial_data {
	int number_coders;
	int time_burnout;
	int time_compile;
	int time_debug;
	int time_refactor;
	int compiles_required;
	int dongle_cooldown;
	enum e_scheduler scheduler;
} t_initial_data;

typedef struct s_coder {
	
	int id;
	enum e_state status;
	int last_compile_start;
	int dongle_left;
	int dongle_rigth;
	int holds_left;
	int holds_rigth;
	int compiles_done;
	pthread_t thread;

} t_coder;


t_initial_data *parsing(char **av);
void display_initial_data(t_initial_data* data);

#endif
