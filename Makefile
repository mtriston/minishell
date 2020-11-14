# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kdahl <kdahl@student.21-school.ru>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/05 20:19:48 by kdahl             #+#    #+#              #
#    Updated: 2020/11/13 23:50:59 by mtriston         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = \
main.c \
builtins.c \
exec.c \
exec_cmd.c \
error.c \
export.c \
export_cont.c \
unset.c \
lexer/lexer.c \
lexer/lexer_utils.c \
lexer/prepare_line.c \
lexer/read_line.c \
lexer/token_list_utils.c \
lexer/validate_line.c \
parser/cmd_list_utils.c \
parser/parse_redirects.c \
parser/parser.c \
parser/split_pipe.c \
signals.c \
launching_cmd.c\

OBJS = $(SRCS:.c=.o)

CC = gcc -g -Wall -Wextra -Werror

all:	$(NAME)

%.o : %.c
			$(CC) -c $< -o $@

$(NAME):	$(OBJS)
			make -C libft
			$(CC) $(OBJS) -L ./libft -lft -o $(NAME)

clean:
			rm -rf $(OBJS)
			$(MAKE) clean -C ./libft

fclean:	clean
		rm -rf $(NAME)
		$(MAKE) fclean -C ./libft

re:		fclean all

.PHONY:	all clean fclean re