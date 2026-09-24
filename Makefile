# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nedo-nas <nedo-nas@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/14 10:45:24 by nedo-nas          #+#    #+#              #
#    Updated: 2026/09/24 16:04:36 by nedo-nas         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion

CC = gcc -g 

FLAGS = -Wall -Wextra -Werror

HEADER = codexion.h

SOURCES = srcs/parsing.c srcs/helpers/helpers.c srcs/codexion.c srcs/coders.c srcs/helpers/gettime.c srcs/coder_routine.c

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

.PHONY: all clean fclean re bonus