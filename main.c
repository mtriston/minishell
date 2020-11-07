/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 21:54:35 by mtriston          #+#    #+#             */
/*   Updated: 2020/11/07 15:14:02 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_env	g_env;

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

int	execute_cmd(t_cmd *cmd, t_exec exec)
{
	g_env.pid = fork();
	printf("%d\n", g_env.pid);
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
	{
		wait_child(g_env.pid, &g_env.status);
	}

	return (1);
	// всегда 1, не будет работать exit. костыль,
	// т.к. не сделана нормальная обработка статусов
}

static int	execute_line(char *cmd_line)
{
	t_cmd	*cmd;
	t_exec 	exec;

	exec.fd_out = 1;
	exec.fd_in = 0;
	while (*cmd_line)
	{
		cmd = NULL;
		cmd_line = parse_next_cmd(cmd_line, &cmd, g_env.env);
		while (cmd)
		{
			if (cmd->next)
			{
				pipe(exec.fd_pipe);
				exec.fd_out = exec.fd_pipe[1];
				g_env.status = execute_cmd(cmd,exec);
				close(exec.fd_pipe[1]);
				exec.fd_in = exec.fd_pipe[0];
				exec.fd_out = 1;
			}
			else
			{
				g_env.status = execute_cmd(cmd, exec);
				exec.fd_in = 0;
			}
			printf("status %d\n", g_env.status);
			g_env.pid = 0;
			cmd = cmd->next;
		}
//		destroy_cmd(&cmd);
	}
	return (SUCCESS);
}

static void print_prompt()
{
	char	*path;
	char	*home;
	int 	home_len;

	path = getcwd(NULL, 0);
	home = ft_getenv("HOME", g_env.env);
	home_len = ft_strlen(home);
	ft_putstr_fd("\033[1m \033[31m ", 1);
	ft_putstr_fd(ft_getenv("USERNAME", g_env.env), 1);
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

void		shell_loop()
{
	char 	*cmd_line;
	int 	status;

	status = SUCCESS;
	while (status == SUCCESS)
	{
		cmd_line = NULL;
		print_prompt();
		cmd_line = read_line();
		if (!cmd_line)
			continue;
		status = execute_line(cmd_line);
		free_gc(cmd_line);
	}
	free_gc(NULL);
}

int		main(int argc, char **argv, char **envp)
{
	g_env.env = envp;

	shell_loop();
	return(0);
}