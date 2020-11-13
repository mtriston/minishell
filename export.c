/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdahl <kdahl@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 19:40:18 by kdahl             #+#    #+#             */
/*   Updated: 2020/11/13 19:20:36 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		export_print_env(char **envp)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[j + 1])
		{
			if (ft_strcmp(envp[j], envp[j + 1]) > 0)
			{
				temp = envp[j];
				envp[j] = envp[j + 1];
				envp[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
	while (*envp)
	{
		ft_putstr_fd("declare -x  ", 1);
		ft_putendl_fd(*envp++, 1);
	}
}

int			change_env(char *str)
{
	int		i;
	int		len;
	int		flag;

	i = 0;
	len = ft_found(str, '=');
	flag = len == -1 ? 1 : 0;
	len = len == -1 ? (int)ft_strlen(str) : len;
	while (g_env.env[i])
	{
		if (ft_strncmp(str, g_env.env[i], len) == 0)
		{
			if (flag == 0)
			{
				free(g_env.env[i]);
				g_env.env[i] = env_strdup(str);
			}
			return (1);
		}
		i++;
	}
	return (0);
}

int			validation(t_cmd *cmd, int i)
{
	if (!is_valid_name(cmd->args[i]))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(cmd->args[i], 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	return (0);
}

void		export_body(int i, t_cmd *cmd)
{
	int		j;
	int		k;
	char	**tab;

	j = 0;
	k = 0;
	tab = NULL;
	tab = malloc((envp_len(g_env.env) + 2) * sizeof(char *));
	while (g_env.env[k])
		tab[j++] = env_strdup(g_env.env[k++]);
	tab[j++] = env_strdup(cmd->args[i]);
	tab[j] = NULL;
	ft_free_array(g_env.env, free);
	g_env.env = tab;
}

int			cmd_export(t_cmd *cmd, char **envp)
{
	int		status;
	int		i;

	i = 1;
	status = 0;
	if (cmd->args[1] == NULL)
		export_print_env(envp);
	else
	{
		while (cmd->args[i])
		{
			status = validation(cmd, i);
			if (change_env(cmd->args[i]) == 0)
				export_body(i, cmd);
			i++;
		}
	}
	return (status);
}
