# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nedo-nas <nedo-nas@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/14 10:45:24 by nedo-nas          #+#    #+#              #
#    Updated: 2026/09/25 15:12:34 by nedo-nas         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion

CC = cc 

FLAGS = -Wall -Wextra -Werror -pthread -g

HEADER = codexion.h

SOURCES = srcs/parsing.c srcs/helpers/utils.c srcs/helpers/utils_parsing.c srcs/main.c \
		  srcs/init.c srcs/simulation.c srcs/monitor.c srcs/coder.c srcs/scheduler_heap.c srcs/helpers/utils_coder.c \
		  srcs/scheduler.c \

OBJS = $(SOURCES:.c=.o)

all: $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

%.o: %.c 
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

format:
	python3 -m c_formatter_42 srcs/init.c

.PHONY: all clean fclean re format