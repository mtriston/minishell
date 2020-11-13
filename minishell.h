/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdahl <kdahl@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/05 20:15:32 by kdahl             #+#    #+#             */
/*   Updated: 2020/11/13 23:25:39 by mtriston         ###   ########.fr       */
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
# include <signal.h>

# define FAILURE 1
# define SUCCESS 0

# define BUILTIN_NUM 7
# define ECHO 0
# define CD 1
# define PWD 2
# define EXPORT 3
# define UNSET 4
# define ENV 5
# define EXIT 6

typedef struct		s_exec
{
	int				fd_in;
	int				fd_out;
	int				fd_pipe[2];
}					t_exec;

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
	int				out;
	struct s_cmd	*next;
}					t_cmd;

typedef struct		s_env
{
	char			**env;
	int				status;
	pid_t			pid;
	int				sigint;
	int				sigquit;
}					t_env;

t_env				g_env;

int					main(int argc, char **argv, char **envp);
int					launch_executable(t_cmd *cmd, char **envp);
int					cmd_echo(t_cmd *cmd, char **envp);
int					cmd_cd(t_cmd *cmd, char **envp);
int					cmd_exit(t_cmd *cmd, char **envp);
int					cmd_pwd(t_cmd *cmd, char **envp);
int					cmd_env(t_cmd *cmd, char **envp);
int					ft_perror(char *s, int code);
int					syntax_error(char *token);
int					cmd_export(t_cmd *cmd, char **envp);
int					cmd_unset(t_cmd *cmd, char **envp);
int					ft_found(const char *str, char c);
int					envp_len(char **envp);
int					is_valid_name(char *str);
int					(*g_launch_builtin(int i))(t_cmd *cmd, char **envp);
void				signal_quit(int code);
void				signal_int(int code);
void				execute_line(char *cmd_line);
void				destroy_cmd(t_cmd **lst);
void				wait_child(pid_t pid);
void				print_prompt(void);
char				*env_strdup(char *str);
char				*parse_next_cmd(char *cmd_line, t_cmd **cmd);
char				*ft_getenv(char *arg, char **envp);
char				*read_line();
int					change_env(char *str);

#endif
