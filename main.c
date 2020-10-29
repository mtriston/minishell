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

int	execute_cmd(t_cmd *cmd, char **envp)
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

static int	execute_line(char *cmd_line, char **env)
{
	t_cmd	*cmd;
	int 	status;

	status = SUCCESS;
	while (*cmd_line)
	{
		cmd = NULL;
		cmd_line = parse_next_cmd(cmd_line, &cmd, env);
		// нижний while идет по связному списку cmd, который содержит команды, которые должны быть связаны между собой пайпами. Пайпы нереализованы.
		while (cmd)
		{
			status = execute_cmd(cmd, env);
			cmd = cmd->next;
		}
//		destroy_cmd(&cmd);
	}
	return (status);
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