/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 20:41:24 by mtriston          #+#    #+#             */
/*   Updated: 2020/10/17 17:49:16 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_begin(char *line)
{
	int i;
	int status;

	i = 0;
	status = 0;
	while (line[i] && ft_isspace(line[i]))
		i++;
	if (line[i] == '|')
		ft_perror("syntax error near unexpected token `|'");
	else if (line[i] == ';')
	{
		if (line[i + 1] == ';')
			ft_perror("syntax error near unexpected token `;;'");
		else
			ft_perror("syntax error near unexpected token `;'");
	}
	else
		status = 1;
	return (status);
}

static int	check_end(char **line)
{
	//TODO: handle | and \ at the end of line
	int status;
	int i;

	status = 0;
	i = ft_strlen(*line) - 1;
	while (i >= 0 && ft_isspace((*line)[i]))
		i--;
	if ((*line)[i] == '<' || (*line)[i] == '>')
		ft_perror("syntax error near unexpected token `newline'");
	else if ((*line)[i] == '|' || (*line)[i] == '\\')
	{
		ft_putstr_fd("> ", 1);
		return (read_line(line));
	}
	else
		status = 1;
	return (status);
}

static int		check_semicolon(char *line)
{
	int i;

	i = 0;
	if (line[i] == ';') {
		ft_perror("syntax error near unexpected token `;;'");
		return (0);
	}
	while (ft_isspace(line[i]))
		i++;
	if (line[i] == ';')
	{
		ft_perror("syntax error near unexpected token `;'");
		return (0);
	}
	return (1);
}

int	validate_line(char **line)
{
	int i;
	int status;

	i = 0;
	status = 1;
	if (!check_begin(*line) || !check_end(line))
		return (0);
	while ((*line)[i] && status)
	{
		if ((*line)[i] == '\\')
			i++;
		else if ((*line)[i] == ';')
			status = check_semicolon(&(*line)[i + 1]);
//		else if (line[i] == '|')
//			check_pipe();
//		else if (line[i] == '&')
//			check_ampersand();
		i++;
	}
	return (status);
}

t_token	*init_token(size_t size, t_token *pre_token)
{
	t_token *token;

	token = malloc_gc(sizeof(t_token));
	if (!token)
		exit(EXIT_FAILURE);
	token->data = calloc_gc(size + 1, sizeof(char));
	if (!token->data)
		exit(EXIT_FAILURE);
	token->type = 0;
	token->next = NULL;
	token->prev = pre_token;
	if (pre_token != NULL)
		pre_token->next = token;
	return (token);
}

t_token 	*tokenize(char *line)
{
	t_token	*root;
	t_token *token;
	int		i;
	int		j;
	
	i = 0;
	j = 0;
	root = init_token(ft_strlen(line), NULL);
	token = root;
	while (line[i])
	{
		if (ft_isalnum(line[i]))
			token->data[j++] = line[i];
		else if (line[i] == '\\')
			token->data[j++] = line[++i];
		else if (line[i] == '|')
		{
			token = init_token(ft_strlen(line), token);
			token->type = TYPE_PIPE;
			token = init_token(ft_strlen(line), token);
			j = 0;
		}
		else if (line[i] == ';')
		{
			token = init_token(ft_strlen(line), token);
			token->type = TYPE_BREAK;
			token = init_token(ft_strlen(line), token);
			j = 0;
		}
		else if (ft_isspace(line[i]))
		{
			while (ft_isspace(line[i]))
				i++;
			token = init_token(ft_strlen(line), token);
			j = 0;
		}
		else if (line[i] == '>')
		{
			token = init_token(ft_strlen(line), token);
			token->type = TYPE_BIGGER;
			token = init_token(ft_strlen(line), token);
			j = 0;
		}
		else if (line[i] == '<')
		{
			token = init_token(ft_strlen(line), token);
			token->type = TYPE_LOWER;
			token = init_token(ft_strlen(line), token);
			j = 0;
		}
		i++;
	}
	return (root);
}

size_t	tok_list_size(t_token *list)
{
	int i;

	i = 0;
	while (list)
	{
		i++;
		list = list->next;
	}
	return (i); 
}
/*
t_token 	*lexer(char *line)
{
	t_token	*tokens;
	size_t	i;
	
	i = 0;
	if (!line)
		return (0);
	if (!validate(line))
		return (0);
	tokens = tokenize(line);
	return (1);
}
*/