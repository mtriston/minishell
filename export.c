/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdahl <kdahl@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 19:40:18 by kdahl             #+#    #+#             */
/*   Updated: 2020/11/12 00:19:34 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char 		*env_strdup(char *str)
{
	char	*dest;

	if (!(dest = (char *)malloc((ft_strlen(str) + 1) * sizeof(char))))
		exit(EXIT_FAILURE);
	dest = ft_strcpy(dest, str);
	return (dest);
}

int				ft_found(const char *str, char c)
{
	int i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	if (str[i] == c)
		return (i);
	return (-1);
}

int				envp_len(char **envp)
{
	int len;

	len = 0;
	while (envp[len])
		len++;
	return (len);
}

int		is_valid_name(char *str)
{
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	while (*str && *str != '=')
	{
		if (!ft_isalnum(*str) && *str != '_')
			if (!(*str == '+' && *(str + 1) && *(str + 1) == '='))
				return (0);
		str++;
	}
	return (1);
}

void		export_print_env(char **envp)
{
	int i;
	int j;
	char *temp;
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
		ft_putstr_fd("declare -x ", 1);
		ft_putendl_fd(*envp++, 1);
	}
}

int cmd_export(t_cmd *cmd, char **envp)
{
	int i;
	int j;
	int k;
	int status;
	char **tab;

	i = 1;
	status = 0;
	tab = NULL;
	if (cmd->args[1] == NULL)
		export_print_env(envp);
	else
	{
		while (cmd->args[i])
		{
			//export te дважды выоводит te
			//export существующей переменной должен обновить ее значение
			if (!is_valid_name(cmd->args[i]) || !ft_strchr(cmd->args[i], '='))
			{
				ft_putstr_fd("export: `", 2);
				ft_putstr_fd(cmd->args[i], 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
				status = 1;
			}
			else
			{
				j = 0;
				k = 0;
				j = 0;
				tab = malloc((envp_len(g_env.env) + 2) * sizeof(char *));
				while (g_env.env[k])
					tab[j++] = env_strdup(g_env.env[k++]);
				tab[j++] = env_strdup(cmd->args[i]);
				tab[j] = NULL;
				ft_free_array(g_env.env, free);
				g_env.env = tab;
			}
			i++;
		}
	}
	return (status);
}
