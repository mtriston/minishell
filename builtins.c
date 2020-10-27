#include "minishell.h"

char *ft_getenv(char *arg, char **envp)
{
	while (*envp)
	{
		if (ft_strncmp(*envp, arg, ft_found(*envp, '=')) == 0)
			return (*envp + ft_strlen(arg) + 1);
		envp++;
	}
	return ("");
}

int cmd_echo(t_cmd *cmd, char **envp)
{
	//TODO: Сейчас работает неверно из-за недоделанного парсера
	int		n_flag;
	size_t	i;

	n_flag = 0;
	i = 0;
	if (cmd->args[0] != NULL)
	{
		if (ft_strncmp(cmd->args[0], "-n", 2) == 0)
			n_flag = 1;
		while (cmd->args[++i])
		{
			ft_putstr_fd(cmd->args[i], 1);
			if (cmd->args[i + 1] != NULL)
				ft_putstr_fd(" ", 1);
		}
		if (!n_flag)
			ft_putstr_fd("\n", 1);
	}
		return(1);
}

int cmd_cd(t_cmd *cmd, char **envp)
{
	char *dir;

	if (cmd->args[1] == NULL)
		dir = ft_getenv("HOME", envp);
	else
		dir = cmd->args[1];
	if (chdir(dir) != 0)
		ft_perror(NULL);
	//cmd_export("PWD", getcwd(NULL, 0), envp);
	return (1);
}

int	cmd_env(t_cmd *cmd, char **envp)
{
	while (*envp)
	{
		ft_putendl_fd(*envp, 1);
		envp++;
	}
	return (1);
}

int	cmd_exit(t_cmd *cmd, char **envp)
{
	free_gc(NULL);
	exit(EXIT_SUCCESS);
}

int	cmd_pwd(t_cmd *cmd, char **envp)
{
	char buf[PATH_MAX];

	getcwd(buf, PATH_MAX);
	ft_putendl_fd(buf, 1);
	return (1);
}