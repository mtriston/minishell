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
	char	**buf;
	int		i;
	int		j;

	i = -1;
	j = -1;
	temp = ft_calloc(envp_len(envp), sizeof(char *));
	while (envp[i++])
	{
		if (ft_strncmp(arg, envp[i], ft_found(envp[i], '=')) != 0)
		{
			if (!(temp[++j] = ft_strdup(envp[i])))
				exit (1);
		}
	}
	buf = envp;
	envp = temp;
	//free
	return ;
}

static int	pars_unset(char *str, char **envp)
{
	int i;
	int	len;

	i = -1;
	len = ft_strlen(str);
	if (ft_strchr(str, '\n') || ft_found(str, '=') != -1)
	{
		write(2, "minishell: ", 11);
		write(2, str, len);
		write(2, ": not a valid identifier\n", 26);
		return (0);
	}
	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(str, envp[i], len) == 0)
			return (1);
	}
	return (0);
}

int     cmd_unset(t_cmd *cmd, char **envp)
{
    int     i;
    i = 0;
    errno = 0;
    while (cmd->args[i])
    {
        //if (quotes)
        //  return ;
        if (pars_unset(cmd->args[i], envp))
            delete_arg(cmd->args[i], envp);
        i++;
    }
    return (1);
}