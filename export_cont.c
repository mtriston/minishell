/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cont.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdahl <kdahl@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 16:24:48 by kdahl             #+#    #+#             */
/*   Updated: 2020/11/13 16:27:19 by kdahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*env_strdup(char *str)
{
	char	*dest;

	if (!(dest = (char *)malloc((ft_strlen(str) + 1) * sizeof(char))))
		exit(EXIT_FAILURE);
	dest = ft_strcpy(dest, str);
	return (dest);
}

int			ft_found(const char *str, char c)
{
	int i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	if (str[i] == c)
		return (i);
	return (-1);
}

int			envp_len(char **envp)
{
	int len;

	len = 0;
	while (envp[len])
		len++;
	return (len);
}

int			is_valid_name(char *str)
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

char		*ft_getenv(char *arg, char **envp)
{
	if (ft_strcmp(arg, "?") == 0)
		return (ft_itoa(g_env.status));
	while (*envp)
	{
		if (ft_strncmp(*envp, arg, ft_found(*envp, '=')) == 0)
			return (*envp + ft_strlen(arg) + 1);
		envp++;
	}
	return ("");
}
