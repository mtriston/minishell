/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/05 20:15:32 by kdahl             #+#    #+#             */
/*   Updated: 2020/10/29 19:17:49 by mtriston         ###   ########.fr       */
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

# define FAILURE 0
# define SUCCESS 1

# define BUILTIN_NUM 7
# define ECHO 0
# define CD 1
# define PWD 2
# define EXPORT 3
# define UNSET 4
# define ENV 5
# define EXIT 6

typedef struct		s_token
{
	char			*data;
	int				type;
	struct s_token	*next;
}					t_token;

typedef struct		s_cmd
{
	char			*name;
	char			**args;
	int				in;
	int 			out;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}					t_cmd;

int		main(int argc, char **argv, char **envp);
char    **split_line(char const *s);
int		launch_executable(t_cmd *cmd, char **envp);
char	*ft_getenv(char *arg, char **envp);
int		cmd_echo(t_cmd *cmd, char **envp);
int		cmd_cd(t_cmd *cmd, char **envp);
int		cmd_exit(t_cmd *cmd, char **envp);
int		cmd_pwd(t_cmd *cmd, char **envp);
int		cmd_env(t_cmd *cmd, char **envp);
void	ft_perror(char *s);
char	*read_line();
int		syntax_error(char *token);
char 		*parse_next_cmd(char *cmd_line, t_cmd **cmd, char **env);
int			cmd_export(t_cmd *cmd, char **envp);
int			cmd_unset(t_cmd *cmd, char **envp);
int			ft_found(char *str, char c);
int			envp_len(char **envp);
#endif