/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 21:09:22 by mtriston          #+#    #+#             */
/*   Updated: 2020/11/07 13:16:21 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "lexer.h"

/*
 * Читается STDIN, затем проверяется корректность ввода.
 * Сначала проверяются синтаксические ошибки, затем, если в конце строки
 * есть символ | или \, то предлагается продолжить ввод.
 */

static int	check_begin(char *line)
{
	while (*line && ft_isspace(*line))
		line++;
	if (*line == '|')
		return (syntax_error("|"));
	if (*line == ';')
	{
		if (*(line + 1) == ';')
			return (syntax_error(";;"));
		else
			return (syntax_error(";"));
	}
	return (VALID_LINE);
}

static int	check_end(char *line)
{
	size_t i;
	int		continue_input;

	i = ft_strlen(line);
	continue_input = 0;
	i = i == 0 ? i : i - 1;
	if (i >= 1 && line[i] == '\\' && line[i - 1] != '\\')
		continue_input = 1;
	while (i >= 0 && ft_isspace(line[i]))
		i--;
	if (line[i] == '<' || line[i] == '>')
		return (syntax_error("newline"));
	if (line[i] == '|' || continue_input)
	{
		ft_putstr_fd("> ", 1);
		return (CONTINUE_INPUT);
	}
	return (VALID_LINE);
}

static int		check_next_char(char c, char *line)
{
	if (c == ';' && *line == ';')
		return (syntax_error(";;"));
	if (c == '>' && *line == '<')
		return (syntax_error("<"));
	if (c == '<' && *line == '>')
		return (syntax_error(">"));
	if (c == '>' && *(++line) == '>')
		return (VALID_LINE);
	while (ft_isspace(*line))
		line++;
	if ((c == '>' || c == '<') && *line == '>')
		return (syntax_error(">"));
	if ((c == '>' || c == '<') && *line == '<')
		return (syntax_error("<"));
	if (*line == ';')
		return (syntax_error(";"));
	if (*line == '|' && (c == ';' || c == '<' || c == '|'))
		return (syntax_error("|"));
	return (VALID_LINE);
}

int	validate_line(char *line)
{
	int status;
	int in_quote;
	int in_dquote;
	int i;

	status = VALID_LINE;
	in_quote = 0;
	in_dquote = 0;
	i = 0;
	if (check_begin(line) == SYNTAX_ERROR)
		return (SYNTAX_ERROR);
	while (line[i] != '\0' && status == VALID_LINE)
	{
		if (line[i] == '\\')
			i++;
		else if (line[i] == '\"' && !in_quote)
			in_dquote = !in_dquote;
		else if (line[i] == '\'' && !in_dquote)
			in_quote = !in_quote;
		else if (ft_strchr("<>|;", line[i]) && !in_quote && !in_dquote)
			status = check_next_char(line[i], &line[i + 1]);
		i++;
	}
	if (status == SYNTAX_ERROR)
		return (SYNTAX_ERROR);
	status = check_end(line);
	return (status);
}