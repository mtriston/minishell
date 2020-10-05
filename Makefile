# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kdahl <kdahl@student.21-school.ru>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/05 20:19:48 by kdahl             #+#    #+#              #
#    Updated: 2020/10/05 20:26:34 by kdahl            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = main.c split_line.c

OBJS = $(SRCS:.c = .o)


CC = gcc ${SRCS}

all:	$(NAME)

$(NAME):	$(OBJS)
			$(CC) -L ./libft -lft -o $(NAME)
