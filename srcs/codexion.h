/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   codexion.h                                        :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/28 18:09:53 by username         #+#    #+#              */
/*   Updated: 2026/09/29 14:11:01 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

enum e_scheduler
{
	EDF,
	FIFO,
};

enum e_state
{
	TAKING_DONGLE,
	COMPILING,
	DEBUGGING,
	REFACTORING,
	DEAD,
};

typedef struct s_dongle
{
	int				id;
	int				is_busy;
	long			available_at;
	pthread_mutex_t	mutex;
}	t_dongle;

typedef struct s_table	t_table;

typedef struct s_coder
{
	int				id;
	int				compiles_done;
	int				holds_left;
	int				holds_rigth;
	enum e_state	status;
	long			last_compile_start;
	t_dongle		*dongle_left;
	t_dongle		*dongle_rigth;
	pthread_t		thread;
	pthread_mutex_t	status_coder;
	t_table			*table;

}	t_coder;

typedef struct s_table
{
	int					number_coders;
	int					time_burnout;
	int					time_compile;
	int					time_debug;
	int					time_refactor;
	int					compiles_required;
	int					dongle_cooldown;
	int					simulation_finish;
	int					current_coder;
	long				start_time;
	enum e_scheduler	scheduler;
	pthread_mutex_t		log_mutex;
	pthread_mutex_t		stop_mutex;
	pthread_t			monitor;
	t_dongle			*dongles;
	t_coder				*coders;
}	t_table;

t_dongle	*smallest_dongle(t_dongle *d1, t_dongle *d2);
t_dongle	*large_dongle(t_dongle *d1, t_dongle *d2);

int		parsing(t_table *table, char **av);
int		is_integer(long number);
int		is_valid_number(char *av);
int		ft_atol(char *str);
int		iniciate_table(t_table *table);
int		simulation(t_table *table);
int		simulation_over(t_table *table);
long	current_time_ms(void);
void	print_table(t_table *data);
void	log_state(int id, char *message, t_table *table);
void	*coder_routine(void *arg);
void	*monitor_routine(void *arg);
void	destroy_mutexes(t_table *table);

#endif
