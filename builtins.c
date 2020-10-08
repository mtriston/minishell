#include "minishell.h"

static char *get_env(char *arg, char **envp)
{
	char *temp;

	while (*envp)
	{
		if ((temp = ft_strnstr(*envp, arg, ft_strlen(*envp))))
		{
			return (temp + ft_strlen(arg) + 1);
		}
		envp++;
	}
	return ("");
}

int cmd_echo(char **args, char **envp)
{
	if (args[1] == NULL)
		ft_putstr_fd("\n", 1);
	else if (ft_strncmp(args[1], "-n", 2) == 0)
		ft_putstr_fd(args[2], 1);
	else
	{
		while (*++args)
		{
			if (*args[0] == '$')
				*args = get_env(*args + 1, envp);
			ft_putstr_fd(*args, 1);
			if (*args + 1 != NULL)
				ft_putstr_fd(" ", 1);
		}
		ft_putstr_fd("\n", 1);
	}
		return(1);
}

int cmd_cd(char **args, char **envp)
{
	char *dir;

	if (args[1] == NULL)
		dir = get_env("HOME", envp);
	else
		dir = args[1];
	if (chdir(dir) != 0)
		ft_putendl_fd("No such directory", 1);
	return (1);
}

int	cmd_env(char **args, char **envp)
{
	// Не обновляет окружени при переходе в другие папки

	while (*envp)
	{
		ft_putstr_fd(*envp, 1);
		write(1, "\n", 1);
		envp++;
	}
	return (1);
}

int	cmd_exit(char **args, char **envp)
{
	exit(0);
	return (0);
}

int	cmd_pwd(char **args, char **envp)
{
	char buf[200];

	getcwd(buf, 200);
	ft_putstr_fd(buf, 1);
	write(1, "\n", 1);
	return (1);
}