/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdahl <kdahl@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/24 22:21:25 by kdahl             #+#    #+#             */
/*   Updated: 2020/10/27 18:20:10 by kdahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	delete_arg(char *arg, char **envp)
{
	char	**temp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	temp = ft_calloc(envp_len(envp), sizeof(char *));
	while (envp[i])
	{
		if (ft_strncmp(arg, envp[i], ft_found(envp[i], '=')) != 0)
			temp[j++] = ft_strdup(envp[i]);
		i++;
	}
	g_env.env = temp;
}

static int	pars_unset(char *str, char **envp)
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

int     cmd_unset(t_cmd *cmd, char **envp)
{
    int     i;

    i = 0;
    while (cmd->args[i])
    {
        if (pars_unset(cmd->args[i], envp))
            delete_arg(cmd->args[i], envp);
        i++;
    }
    return (1);
}
