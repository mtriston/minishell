#include "../../includes/minishell.h"

static void		delete_arg(char *arg, char **envp)
{
	char	**temp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	temp = malloc(envp_len(envp) * sizeof(char *));
	while (envp[i])
	{
		if (ft_strncmp(arg, envp[i], ft_found(envp[i], '=')) != 0)
			temp[j++] = env_strdup(envp[i]);
		i++;
	}
	temp[j] = NULL;
	ft_free_array(g_env.env, free);
	g_env.env = temp;
}

static int		pars_unset(char *str, char **envp)
{
	int i;
	int	len;

	i = 0;
	len = ft_strlen(str);
	while (envp[i])
	{
		if (ft_strncmp(str, envp[i], len) == 0)
			return (1);
		i++;
	}
	return (0);
}

int				cmd_unset(t_cmd *cmd, char **envp)
{
	int		i;
	int		status;

	(void)envp;
	i = 1;
	status = 0;
	while (cmd->args[i])
	{
		if (is_valid_name(cmd->args[i]) == 0)
		{
			ft_putstr_fd("minishell: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			status = 1;
		}
		else if (pars_unset(cmd->args[i], g_env.env))
			delete_arg(cmd->args[i], g_env.env);
		i++;
	}
	return (status);
}
