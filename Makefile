# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kdahl <kdahl@student.21-school.ru>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/05 20:19:48 by kdahl             #+#    #+#              #
#    Updated: 2020/10/21 20:28:02 by mtriston         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = main.c builtins.c exec.c error.c read_line.c parser.c lexer.c lexer_utils.c export.c unset.c

OBJS = $(SRCS:.c = .o)


CC = gcc -g ${SRCS}

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