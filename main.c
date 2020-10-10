#include "minishell.h"

int		(*launch_builtin(int i))(char **args, char **envp)
{
	int (*launch_builtin[BUILTIN_NUM])(char **args, char **envp);

	launch_builtin[ECHO] = &cmd_echo;
	launch_builtin[CD] = &cmd_cd;
	launch_builtin[PWD] = &cmd_pwd;
	//launch_builtin[EXPORT] = &cmd_export;
	//launch_builtin[UNSET] = &cmd_unset;
	launch_builtin[ENV] = &cmd_env;
	launch_builtin[EXIT] = &cmd_exit;
	return (launch_builtin[i]);
}

/*
 *  TODO: заменить strncmp на strcmp для точной проверки на равенство.
 */

int	execute(char *command, char **envp)
{
	char	**args;
	int		status;

	args = split_line(command);
	status = 1;
	if (ft_strncmp(args[0], "echo,", 4) == 0)
		status = launch_builtin(ECHO)(args, envp);
	else if (ft_strncmp(args[0], "cd,", 2) == 0)
		status = launch_builtin(CD)(args, envp);
	else if (ft_strncmp(args[0], "pwd,", 3) == 0)
		status = launch_builtin(PWD)(args, envp);
	else if (ft_strncmp(args[0], "export,", 6) == 0)
		status = launch_builtin(EXPORT)(args, envp);
	else if (ft_strncmp(args[0], "unset,", 5) == 0)
		status = launch_builtin(UNSET)(args, envp);
	else if (ft_strncmp(args[0], "env,", 3) == 0)
		status = launch_builtin(ENV)(args, envp);
	else if (ft_strncmp(args[0], "exit,", 4) == 0)
		status = launch_builtin(EXIT)(args, envp);
	else
		status = launch_executable(args, envp);
	return (status);
}

static void print_prompt()
{
	char buf[PATH_MAX];

	getcwd(buf, PATH_MAX);
	ft_putstr_fd("\033[1m \033[31m minishell:", 1);
	ft_putstr_fd(buf, 1);
	ft_putstr_fd("$ \033[0m", 1);
}

void		shell_loop(char **envp)
{
	char	*line;
	char	**commands;
	int		status;

	status = 1;
	while (status)
	{
		print_prompt();
		get_next_line(0, &line);
		commands = ft_split(line, ';');
		while (status && commands && *commands)
		{
			status = execute(*commands, envp);
			commands++;
		}
		free_gc(NULL);
		line = NULL;
	}
	free_gc(line);
}

int	main(int argc, char **argv, char **envp)
{
	shell_loop(envp);

	return(0);
}