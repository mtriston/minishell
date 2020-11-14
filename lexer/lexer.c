/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 19:32:51 by mtriston          #+#    #+#             */
/*   Updated: 2020/11/13 23:11:54 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "lexer.h"

static void	handle_backslash(char **line, t_token **token)
{
	(*line)++;
	ft_strlcat((*token)->data, *line, ft_strlen((*token)->data) + 2);
	(*line)++;
}

static void	handle_quote(char **line, t_token **token)
{
	size_t i;

	i = 0;
	(*line)++;
	while ((*line)[i] && (*line)[i] != '\'')
		i++;
	i++;
	ft_strlcat((*token)->data, *line, ft_strlen((*token)->data) + i);
	*line = *line + i;
}

static void	handle_dquote(char **line, t_token **token)
{
	int in_qoute;

	in_qoute = -1;
	(*line)++;
	while (**line && **line != '\"')
	{
		if (**line == '\'')
			in_qoute *= -1;
		if (**line == '\\' && in_qoute == -1)
			handle_backslash(line, token);
		else
		{
			ft_strlcat((*token)->data, *line, ft_strlen((*token)->data) + 2);
			*line += 1;
		}
	}
	(*line) = **line ? (*line) + 1 : (*line);
}

static void	handle_general(char **line, t_token **token)
{
	size_t	i;

	i = 0;
	while ((*line)[i] && !ft_strchr("<>|\'\"\\", (*line)[i]) && \
											!ft_isblank((*line)[i]))
		i++;
	ft_strlcat((*token)->data, *line, ft_strlen((*token)->data) + i + 1);
	*line = *line + i;
}

t_token		*lexer(char *line)
{
	t_token *root;
	t_token *current;
	size_t	data_size;

	line = prepare_line(line);
	data_size = ft_strlen(line);
	root = token_init(data_size, NULL);
	current = root;
	while (*line)
	{
		if (*line == '>' || *line == '<')
			handle_redirect(&line, &current, data_size);
		else if (*line == '\'')
			handle_quote(&line, &current);
		else if (*line == '\"')
			handle_dquote(&line, &current);
		else if (*line == '\\')
			handle_backslash(&line, &current);
		else if (ft_isblank(*line))
			handle_blank(&line, &current, data_size);
		else if (!ft_strchr("<>|\'\"\\", *line) && !ft_isblank(*line))
			handle_general(&line, &current);
	}
	return (root);
}
