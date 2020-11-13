/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdahl <kdahl@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 16:37:50 by kdahl             #+#    #+#             */
/*   Updated: 2020/11/13 22:49:20 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_cmd_in_child(t_cmd *cmd, char **envp)
{
	g_env.status = SUCCESS;
	if (cmd->name == NULL)
		return (FAILURE);
	if (ft_strcmp(cmd->name, "echo") == 0)
		g_env.status = g_launch_builtin(ECHO)(cmd, envp);
	else if (ft_strcmp(cmd->name, "cd") == 0)
		g_env.status = g_launch_builtin(CD)(cmd, envp);
	else if (ft_strcmp(cmd->name, "pwd") == 0)
		g_env.status = g_launch_builtin(PWD)(cmd, envp);
	else if (ft_strcmp(cmd->name, "export") == 0)
		g_env.status = g_launch_builtin(EXPORT)(cmd, envp);
	else if (ft_strcmp(cmd->name, "unset") == 0)
		g_env.status = g_launch_builtin(UNSET)(cmd, envp);
	else if (ft_strcmp(cmd->name, "env") == 0)
		g_env.status = g_launch_builtin(ENV)(cmd, envp);
	else
		launch_executable(cmd, envp);
	return (g_env.status);
}

static int	execute_cmd_in_parent(t_cmd *cmd)
{
	if (ft_strcmp(cmd->name, "exit") == 0)
		g_env.status = g_launch_builtin(EXIT)(cmd, g_env.env);
	else if (ft_strcmp(cmd->name, "cd") == 0)
		g_env.status = g_launch_builtin(CD)(cmd, g_env.env);
	else if (ft_strcmp(cmd->name, "export") == 0 && cmd->args[1])
		g_env.status = g_launch_builtin(EXPORT)(cmd, g_env.env);
	else if (ft_strcmp(cmd->name, "unset") == 0)
		g_env.status = g_launch_builtin(UNSET)(cmd, g_env.env);
	else
		return (0);
	return (1);
}

static void	execute_cmd(t_cmd *cmd, t_exec exec)
{
	if (cmd->in == -1)
		return ;
	if (execute_cmd_in_parent(cmd))
		return ;
	g_env.pid = fork();
	if (g_env.pid < 0)
		exit(EXIT_FAILURE);
	if (g_env.pid == 0)
	{
		if (cmd->out != 1)
			exec.fd_out = cmd->out;
		if (cmd->in != 0)
			exec.fd_in = cmd->in;
		dup2(exec.fd_in, 0);
		dup2(exec.fd_out, 1);
		g_env.status = execute_cmd_in_child(cmd, g_env.env);
		exit(g_env.status);
	}
	if (g_env.pid > 0)
		wait_child(g_env.pid);
}

static void	execute_with_pipe(t_cmd *cmd, t_exec *exec)
{
	pipe(exec->fd_pipe);
	exec->fd_out = exec->fd_pipe[1];
	execute_cmd(cmd, *exec);
	close(exec->fd_pipe[1]);
	exec->fd_in = exec->fd_pipe[0];
	exec->fd_out = 1;
}

void		execute_line(char *cmd_line)
{
	t_cmd	*cmd;
	t_exec	exec;

	exec.fd_out = 1;
	exec.fd_in = 0;
	while (*cmd_line)
	{
		cmd = NULL;
		cmd_line = parse_next_cmd(cmd_line, &cmd);
		while (cmd)
		{
			if (cmd->next)
				execute_with_pipe(cmd, &exec);
			else
			{
				execute_cmd(cmd, exec);
				exec.fd_in = 0;
			}
			g_env.pid = 0;
			cmd = cmd->next;
		}
		destroy_cmd(&cmd);
	}
}
