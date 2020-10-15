/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 20:41:24 by mtriston          #+#    #+#             */
/*   Updated: 2020/10/15 23:06:38 by mtriston         ###   ########.fr       */
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

static int	check_end(char *line)
{
	int status;
	int i;

	status = 0;
	i = ft_strlen(line) - 1;
	while (i >= 0 && ft_isspace(line[i]))
		i--;
	if (line[i] == '<' || line[i] == '>')
		ft_perror("syntax error near unexpected token `newline'");
	else if (line[i] == '|')
	{

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

static int		validate(char *line)
{
	int i;
	int status;

	i = 0;
	status = 1;
	if (!check_begin(line) || !check_end(line))
		return (0);
	while (line[i] && status)
	{
		if (line[i] == '\\')
			i++;
		else if (line[i] == ';')
			status = check_semicolon(&line[i + 1]);
//		else if (line[i] == '|')
//			check_pipe();
//		else if (line[i] == '&')
//			check_ampersand();
		i++;
	}
	return (status);
}

void 	tokenize(char *line)
{

}

int 	lexer(char *line)
{
	if (!line)
		return (0);
	if (!validate(line))
		return (0);
//	tokenize(line);
	return (1);
}