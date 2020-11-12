/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 21:54:35 by mtriston          #+#    #+#             */
/*   Updated: 2020/11/11 20:38:51 by mtriston         ###   ########.fr       */
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
	g_env.status = SUCCESS;
	if (cmd->name == NULL)
		return (FAILURE);
	if (ft_strcmp(cmd->name, "echo") == 0)
		g_env.status = launch_builtin(ECHO)(cmd, envp);
	else if (ft_strcmp(cmd->name, "cd") == 0)
		g_env.status = launch_builtin(CD)(cmd, envp);
	else if (ft_strcmp(cmd->name, "pwd") == 0)
		g_env.status = launch_builtin(PWD)(cmd, envp);
	else if (ft_strcmp(cmd->name, "export") == 0)
		g_env.status = launch_builtin(EXPORT)(cmd, envp);
	else if (ft_strcmp(cmd->name, "unset") == 0)
		g_env.status = launch_builtin(UNSET)(cmd, envp);
	else if (ft_strcmp(cmd->name, "env") == 0)
		g_env.status = launch_builtin(ENV)(cmd, envp);
	else
		launch_executable(cmd, envp);
	return (g_env.status);
}

void wait_child(pid_t pid)
{
	pid_t wpid;

	while (1)
	{
		wpid = waitpid(pid, &g_env.status, WUNTRACED);
		if (wpid >= 0)
			break;
	}
	if (WIFEXITED(g_env.status))
		g_env.status = WEXITSTATUS(g_env.status);
	if (g_env.sigint)
	{
		g_env.status = 130;
		g_env.sigint = 0;
	}
	if (g_env.sigquit)
	{
		g_env.status = 131;
		g_env.sigquit = 0;
	}
}
static int	execute_cmd_in_parent(t_cmd *cmd)
{

	if (ft_strcmp(cmd->name, "exit") == 0)
		g_env.status = launch_builtin(EXIT)(cmd, g_env.env);
	else if (ft_strcmp(cmd->name, "cd") == 0)
		g_env.status = launch_builtin(CD)(cmd, g_env.env);
	else if (ft_strcmp(cmd->name, "export") == 0 && cmd->args[1])
		g_env.status = launch_builtin(EXPORT)(cmd, g_env.env);
	else if (ft_strcmp(cmd->name, "unset") == 0)
		g_env.status = launch_builtin(UNSET)(cmd, g_env.env);
	else
		return (0);
	return (1);

}
static void	execute_cmd(t_cmd *cmd, t_exec exec)
{
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

static void destroy_cmd(t_cmd **lst)
{
	t_cmd *ptr;
	t_cmd *temp;

	if (lst && *lst)
	{
		ptr = *lst;
		while (ptr)
		{
			temp = ptr;
			ptr = ptr->next;
			free_gc(temp->name);
			free_gc(temp->next);
			ft_free_array(temp->args, free_gc);
			free_gc(temp);
		}
		*lst = NULL;
	}
}

static void	execute_line(char *cmd_line)
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
				execute_cmd(cmd,exec);
				close(exec.fd_pipe[1]);
				exec.fd_in = exec.fd_pipe[0];
				exec.fd_out = 1;
			}
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

void			print_prompt(void)
{
	char	*path;
	char	*home;
	int 	home_len;

	path = getcwd(NULL, 0);
	home = ft_getenv("HOME", g_env.env);
	home_len = ft_strlen(home);
	ft_putstr_fd("\033[1m \033[31m ", 0);
	ft_putstr_fd(ft_getenv("USER", g_env.env), 0);
	ft_putstr_fd(":", 0);
	if (ft_strncmp(path, home, home_len) == 0)
	{
		ft_putstr_fd("~", 0);
		ft_putstr_fd(path + home_len, 0);
	}
	else
		ft_putstr_fd(path, 0);
	ft_putstr_fd("$ \033[0m", 0);
	free(path);
}

void		shell_loop()
{
	char 	*cmd_line;

	while (1)
	{
		cmd_line = NULL;
		print_prompt();
		cmd_line = read_line();
		if (cmd_line != NULL) {
			execute_line(cmd_line);
			free_gc(cmd_line);
		}
	}
}

void 	env_init(char **env)
{
	size_t i;

	i = 0;
	g_env.status = 0;
	g_env.pid = 0;
	g_env.sigquit = 0;
	g_env.sigint = 0;
	g_env.env = malloc((envp_len(env) + 1) * sizeof(char *));
	if (!g_env.env)
		exit(EXIT_FAILURE);
	while (env[i])
	{
		g_env.env[i] = (char *)malloc((ft_strlen(env[i]) + 1) * sizeof(char));
		if (!g_env.env[i])
			exit(EXIT_FAILURE);
		ft_strcpy(g_env.env[i], env[i]);
		i++;
	}
	g_env.env[i] = NULL;
}

int		main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	signal(SIGINT, &signal_int);
	signal(SIGQUIT, &signal_quit);
	env_init(envp);
	shell_loop();
	return (0);
}
