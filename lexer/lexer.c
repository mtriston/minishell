/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 19:32:51 by mtriston          #+#    #+#             */
/*   Updated: 2020/11/07 18:38:27 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "lexer.h"

static void	handle_quote(char **line, t_token **token, char quote)
{
	size_t i;

	i = 0;
	(*line)++;
	while ((*line)[i] && (*line)[i] != quote)
	{
		if (quote == '\"' && (*line)[i] == '\\')
		{
			ft_strlcat((*token)->data, (*line), ft_strlen((*token)->data) + i + 1);
			*line = *line + i + 1;
			ft_strlcat((*token)->data, (*line), ft_strlen((*token)->data) + 2);
			*line = *line + 1;
			i = 0;
		}
		else
			i++;
	}		
	i = (*line)[i] == '\0' ? i : i + 1;	
	ft_strlcat((*token)->data, *line, ft_strlen((*token)->data) + i);
	*line = *line + i;
}

static void	handle_blank(char **line, t_token **token, int data_size)
{
	size_t i;

	i = 0;
	while ((*line)[i] && ft_isblank((*line)[i]))
		i++;
	if ((*line)[i] && (ft_strlen((*token)->data) > 0))
		*token = token_init(data_size, token);
	*line += i;
}

static void	handle_general(char **line, t_token **token)
{
	size_t	i;

	i = 0;
	while ((*line)[i] && !ft_strchr("<>|\'\"\\", (*line)[i]) \
			&& !ft_isblank((*line)[i]))
		i++;
	ft_strlcat((*token)->data, *line, ft_strlen((*token)->data) + i + 1);
	*line = *line + i;
}

static void	handle_redirect(char **line, t_token **token, int data_size)
{
	if ((*token)->data[0])
		*token = token_init(data_size, token);
	(*token)->type = TYPE_SPECIAL;
	(*token)->data[0] = **line;
	if (**line == '>' && **line == *((*line) + 1))
		(*token)->data[1] = *++*line;
	*token = token_init(data_size, token);
	*line += 1;
}

t_token		*lexer(char *line, char **env)
{
	t_token *root;
	t_token *current;
	size_t	data_size;

	line = prepare_line(line, env);
	data_size = ft_strlen(line);
	root = token_init(data_size, NULL);
	current = root;
	while (*line)
	{
		if (*line == '>' || *line == '<')
			handle_redirect(&line, &current, data_size);
		else if (*line == '\'' || *line == '\"')
			handle_quote(&line, &current, *line);
		else if (*line == '\\')
		{
			ft_strlcat(current->data, ++line, ft_strlen(current->data) + 2);
			line++;
		}
		else if (ft_isblank(*line))
			handle_blank(&line, &current, data_size);
		if (!ft_strchr("<>|\'\"\\", *line) && !ft_isblank(*line))
			handle_general(&line, &current);
	}
	return (root);
}
