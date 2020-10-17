/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/05 20:15:32 by kdahl             #+#    #+#             */
/*   Updated: 2020/10/17 17:17:31 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <dirent.h>
# include <unistd.h>
# include <limits.h>
# include "libft/libft.h"

# define BUILTIN_NUM 7
# define ECHO 0
# define CD 1
# define PWD 2
# define EXPORT 3
# define UNSET 4
# define ENV 5
# define EXIT 6

# define TYPE_PIPE 1
# define TYPE_BREAK 2
# define TYPE_BIGGER 3
# define TYPE_LOWER 4

typedef struct		s_token
{
	char			*data;
	int				type;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

int		main(int argc, char **argv, char **envp);
char    **split_line(char const *s);
int		launch_executable(char **args, char **envp);
char	*ft_getenv(char *arg, char **envp);
int		cmd_echo(char **args, char **envp);
int		cmd_cd(char **args, char **envp);
int		cmd_exit(char **args, char **envp);
int		cmd_pwd(char **args, char **envp);
int		cmd_env(char **args, char **envp);
void	ft_perror(char *s);
t_token *lexer(char *line);
int		validate_line(char **line);
int		read_line(char **line);

#endif