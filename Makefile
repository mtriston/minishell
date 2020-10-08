# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kdahl <kdahl@student.21-school.ru>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/05 20:19:48 by kdahl             #+#    #+#              #
#    Updated: 2020/10/08 00:55:52 by kdahl            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = main.c split_line.c builtins.c

OBJS = $(SRCS:.c = .o)


CC = gcc ${SRCS}

all:	$(NAME)
			./minishell

$(NAME):	$(OBJS)
			make -C libft
			$(CC) -L ./libft -lft -o $(NAME)

clean:
			$(MAKE) clean -C ./libft

fclean:	
		rm -rf $(NAME)
		$(MAKE) fclean -C ./libft

re:		fclean all

.PHONY:	all clean fclean re