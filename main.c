/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 21:54:35 by mtriston          #+#    #+#             */
/*   Updated: 2020/11/04 21:59:35 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int		(*launch_builtin(int i))(t_cmd *cmd, char **envp)
{
	int (*launch_builtin[BUILTIN_NUM])(t_cmd *cmd, char **envp);

	launch_builtin[ECHO] = &cmd_echo;
	launch_builtin[CD] = &cmd_cd;
	launch_builtin[PWD] = &cmd_pwd;
	launch_builtin[EXPORT] = &cmd_export;
	launch_builtin[UNSET] = &cmd_unset;
	launch_builtin[ENV] = &cmd_env;
	launch_builtin[EXIT] = &cmd_exit;
	return (launch_builtin[i]);
}

int execute_cmd_in_child(t_cmd *cmd, char **envp)
{
	int		status;

	status = SUCCESS;
	if (ft_strcmp(cmd->name, "echo") == 0)
		status = launch_builtin(ECHO)(cmd, envp);
	else if (ft_strcmp(cmd->name, "cd") == 0)
		status = launch_builtin(CD)(cmd, envp);
	else if (ft_strcmp(cmd->name, "pwd") == 0)
		status = launch_builtin(PWD)(cmd, envp);
	else if (ft_strcmp(cmd->name, "export") == 0)
		status = launch_builtin(EXPORT)(cmd, envp);
	else if (ft_strcmp(cmd->name, "unset") == 0)
		status = launch_builtin(UNSET)(cmd, envp);
	else if (ft_strcmp(cmd->name, "env") == 0)
		status = launch_builtin(ENV)(cmd, envp);
	else if (ft_strcmp(cmd->name, "exit") == 0)
		status = launch_builtin(EXIT)(cmd, envp);
	else
		status = launch_executable(cmd, envp);
	return (status);
}

void wait_child(pid_t pid, int *status)
{
	pid_t wpid;

	while (1)
	{
		wpid = waitpid(pid, status, WUNTRACED);
		if (wpid >= 0)
			break;
	}
}

int	execute_cmd(t_cmd *cmd, char **envp, t_exec exec)
{
	exec.pid = fork();
	if (exec.pid < 0)
		exit(EXIT_FAILURE);
	if (exec.pid == 0)
	{
		if (cmd->out != 1)
			exec.fd_out = cmd->out;
		if (cmd->in != 0)
			exec.fd_in = cmd->in;
		dup2(exec.fd_in, 0);
		dup2(exec.fd_out, 1);
		exec.status = execute_cmd_in_child(cmd, envp);
		exit(exec.status);
	}
	if (exec.pid > 0)
	{
		wait_child(exec.pid, &exec.status);
	}

	return (1);
	// всегда 1, не будет работать exit. костыль,
	// т.к. не сделана нормальная обработка статусов
}

static int	execute_line(char *cmd_line, char **env)
{
	t_cmd	*cmd;
	t_exec 	exec;

	exec.status = SUCCESS;
	exec.fd_out = 1;
	exec.fd_in = 0;
	while (*cmd_line)
	{
		cmd = NULL;
		cmd_line = parse_next_cmd(cmd_line, &cmd, env);
		while (cmd)
		{
			if (cmd->next)
			{
				pipe(exec.fd_pipe);
				exec.fd_out = exec.fd_pipe[1];
				exec.status = execute_cmd(cmd, env, exec);
				close(exec.fd_pipe[1]);
				exec.fd_in = exec.fd_pipe[0];
				exec.fd_out = 1;
			}
			else
			{
				exec.status = execute_cmd(cmd, env, exec);
				exec.fd_in = 0;
			}
			cmd = cmd->next;
		}
//		destroy_cmd(&cmd);
	}
	return (exec.status);
}

static void print_prompt(char **envp)
{
	char	*path;
	char	*home;
	int 	home_len;

	path = getcwd(NULL, 0);
	home = ft_getenv("HOME", envp);
	home_len = ft_strlen(home);
	ft_putstr_fd("\033[1m \033[31m ", 1);
	ft_putstr_fd(ft_getenv("USERNAME", envp), 1);
	ft_putstr_fd(":", 1);
	if (ft_strncmp(path, home, home_len) == 0)
	{
		ft_putstr_fd("~", 1);
		ft_putstr_fd(path + home_len, 1);
	}
	else
		ft_putstr_fd(path, 1);
	ft_putstr_fd("$ \033[0m", 1);
	free(path);
}

void		shell_loop(char **envp)
{
	char 	*cmd_line;
	int 	status;

	status = SUCCESS;
	while (status == SUCCESS)
	{
		cmd_line = NULL;
		print_prompt(envp);
		cmd_line = read_line();
		if (!cmd_line)
			continue;
		status = execute_line(cmd_line, envp);
		free_gc(cmd_line);
	}
	free_gc(NULL);
}

int	main(int argc, char **argv, char **envp)
{
	shell_loop(envp);
	return(0);
}