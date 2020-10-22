/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 19:32:51 by mtriston          #+#    #+#             */
/*   Updated: 2020/10/22 23:20:26 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

char		*decode_env(char *line, char **envp)
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
	return (decode_env(temp, envp));
}

void 		handle_quote(char **line, t_token **token)
{
	size_t i;

	i = 0;
	(*line)++;
	while ((*line)[i] && (*line)[i] != '\'')
		i++;
	ft_strlcat((*token)->data, *line, ft_strlen((*token)->data) + i + 1);
	*line = *line + i;
}

void		handle_blank(char **line, t_token **token, int data_size)
{
	size_t i;

	i = 0;
	while (ft_isblank((*line)[i]))
		i++;
	if (ft_strlen((*token)-> data) > 0)
		*token = token_init(data_size, token);
	*line += i;
}

void		handle_backslash(char **line, t_token **token)
{
	(*line)++;
	ft_strlcat((*token)->data, *line, ft_strlen((*token)->data) + 2);
	(*line)++;
}

void 		handle_general(char **line, t_token **token)
{
	char *spec_symbols = "<>|;$\'\"\\ ";
	size_t i;

	i = 0;
	while ((*line)[i] && !ft_strchr(spec_symbols, (*line)[i]))
		i++;
	ft_strlcat((*token)->data, *line, ft_strlen((*token)->data) + i + 1);
	*line = *line + i;
}

void 		handle_redirect(char **line, t_token **token, int data_size)
{
	*token = token_init(data_size, token);
	(*token)->type = TYPE_SPECIAL;
	(*token)->data[0] = **line;
	if ((**line == '>' || **line == '<') && **line == *((*line) + 1))
		(*token)->data[1] = *++*line;
	*token = token_init(data_size, token);
	*line += 1;
}

t_token		*lexer(char *line, char **env)
{
	char *spec_symbols = "<>|;$\'\"\\";
	t_token *root;
	t_token *current;
	size_t 	data_size;

	line = decode_env(line, env);
	data_size = ft_strlen(line);
	root = token_init(data_size, NULL);
	current = root;
	while (*line)
	{
		if (*line == '>' || *line == '<' || *line == '|')
			handle_redirect(&line, &current, data_size);
		else if (*line == '\'')
			handle_quote(&line, &current);
		else if (*line == '\\')
			handle_backslash(&line, &current);
		else if (ft_isblank(*line))
			handle_blank(&line, &current, data_size);
		if (!ft_strchr(spec_symbols, *line))
			handle_general(&line, &current);
	}
	return (root);
}