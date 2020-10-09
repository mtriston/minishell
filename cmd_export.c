/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdahl <kdahl@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 19:40:18 by kdahl             #+#    #+#             */
/*   Updated: 2020/10/09 20:00:03 by kdahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** General Env variables
*/

int				envp_len(char **envp)
{
	int len;

	len = 0;
	while (envp[len])
		len++;
	return (len);
}

int				ft_find_char(char *str, char c)
{
	int i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	if (str[i] == c)
		return (i);
	return (-1);
}

static void		free_double(char **str)
{
	int i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
	return ;
}

static char		*check_str(char *str)
{
	int		i;
	char	*rst;

	i = 0;
	while (str[i])
		if (str[i++] == '=')
			return (rst = ft_strdup(str));
	i = -1;
	rst = ft_calloc(ft_strlen(str) + 2, sizeof(char));
	while (str[++i])
		rst[i] = str[i];
	rst[i] = '=';
	return (rst);
}

static void		new_envp2(char *str, char **temp, int i, int j, char **envp)
{
	int sum;

	sum = 0;
	while (envp[++j])
		if (ft_strncmp(str, envp[j], ft_find_char(str, '=') + 1) == 0)
		{
			if (!(temp[j] = ft_strdup(str)))
				exit(1);
			sum = 1;
			break ;
		}
	if (!envp[j])
		if (!(temp[i] = ft_strdup(str)))
			exit(1);
	while (envp[i++])
	{
		if (!(temp[i - sum] = ft_strdup(envp[i - 1])))
			exit(1);
	}
	return ;
}

static void		new_envp(char *str, char **envp)
{
	char	**temp;
	char	**buff;
	int		i;
	int		j;

	i = -1;
	j = -1;
	temp = (char**)ft_calloc(envp_len(envp) + 2, sizeof(char*));
	while (envp[++i] && ft_strncmp("_=", envp[i], 2) != 0)
		if (!(temp[i] = ft_strdup(envp[i])))
			exit(1);
	str = check_str(str);
	new_envp2(str, temp, i, j, envp);
	buff = envp;
	envp = temp;
	free_double(buff);
}

int			cmd_export(char **args, char **envp)
{
	int i;

	i = 0;
	if (!args[1])
	{
		while (envp[i])
		{
			write(1, envp[i], ft_strlen(envp[i]));
			write(1, "\n", 1);
			if (ft_strncmp("__CF", envp[i++], 4) == 0)
				return (1);
		}
		return (1);
	}
	while (args[++i])
		new_envp(args[i], envp);
	return (1);
}