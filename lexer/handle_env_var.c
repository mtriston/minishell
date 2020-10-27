/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 21:23:39 by mtriston          #+#    #+#             */
/*   Updated: 2020/10/27 21:23:39 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "lexer.h"

static int	is_there_env(const char *line)
{
	size_t i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\\')
			i++;
		else if (line[i] == '\'')
		{
			i++;
			while (line[i] && line[i] != '\'')
				i++;
		}
		else if (line[i] == '$')
			return (i);
		i++;
	}
	return (-1);
}

char		*handle_env_var(char *line, char **envp)
{
	char	*begin;
	char	*variable;
	char	*temp;
	size_t	i;

	i = 0;
	if (is_there_env(line) < 0)
		return (line);
	begin = line;
	line = line + is_there_env(line);
	*line = '\0';
	line++;
	while (line[i] && ft_isalnum(line[i]))
		i++;
	variable = ft_substr(line, 0, i);
	line += i;
	temp = ft_strjoin(begin, ft_getenv(variable,envp));
	variable = temp;
	temp = ft_strjoin(variable, line);
	free_gc(variable);
	free_gc(line);
	return (handle_env_var(temp, envp));
}