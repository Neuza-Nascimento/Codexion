/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   codexion.h                                        :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/28 18:09:53 by username         #+#    #+#              */
/*   Updated: 2026/10/08 11:58:50 by username        ###   ########.fr        */
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

typedef struct s_request
{
	int		id_coder;
	int		ticket;
	long	dealine;

}	t_request;

typedef struct s_dongle
{
	int				id;
	int				is_busy;
	long			available_at;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				len_requests;
	t_request		requests[2];

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
	pthread_cond_t	cond_coder;
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
	int					count_tickets;
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

void			push_request(t_dongle *dongle, t_request *new_request, t_table *table);
void			remove_top(t_dongle *dongle, t_table *table);
struct timespec	future_time(long time_in_ms);
void			create_request(t_dongle *dongle, t_coder *coder, t_request *request);

void	iniciate_request(t_coder *coder, int ticket, long deadline, int remove, t_dongle *dongle1, t_dongle *dongle2);
void	leave_dongle(t_dongle *dongle, long cooldown);
int		acquire_dongles(t_dongle *dongle1, t_dongle *dongle2, t_coder *coder);
int		dongle_ok(t_dongle *dongle, t_coder *coder);

int		parsing(t_table *table, char **av);
int		is_integer(long number);
int		is_valid_number(char *av);
int		ft_atol(char *str);
int		iniciate_table(t_table *table);
int		simulation(t_table *table);
int		simulation_over(t_table *table);
long	current_time_ms(void);
void	log_state(int id, char *message, t_table *table);
void	*coder_routine(void *arg);
void	*monitor_routine(void *arg);
void	destroy_mutexes(t_table *table);

#endif
