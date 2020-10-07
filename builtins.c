#include "minishell.h"

int cmd_cd(char **args, char **envp)
{
	char *home_dir;

	if (args[1] == NULL)
	{
		while (*envp)
		{
			if((home_dir = ft_strnstr(*envp, "HOME=", ft_strlen(*envp))))
			{
				chdir(home_dir + 5);
				break;
			}
			envp++;
		}
	}
	else
		if (chdir(args[1]) != 0)
			ft_putendl_fd("No such directory", 1);
	return (1);
}