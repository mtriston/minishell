/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 14:00:33 by mtriston          #+#    #+#             */
/*   Updated: 2020/10/18 19:11:46 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	return (SUCCESS);
}

static int	check_end(char *line)
{
	int i;
	int continue_input;

	i = ft_strlen(line) - 1;
	continue_input = line[i] == '\\' ? 1 : 0;
	while (i >= 0 && ft_isspace(line[i]))
		i--;
	if (line[i] == '<' || line[i] == '>')
		return (syntax_error("newline"));
	if (line[i] == '|' || continue_input)
	{
		ft_putstr_fd("> ", 1);
		return (2);
	}
	return (SUCCESS);
}

static int		check_next_char(char c, char *line)
{
	char *spec_tokens = ";|<>";
	if (c == ';' && *line == ';')
		return (syntax_error(";;"));
	if (c == '>' && *line == '<')
		return (syntax_error("<"));
	if (c == '<' && *line == '>')
		return (syntax_error(">"));
	while (ft_isspace(*line))
		line++;
	if ((c == '>' || c == '<') && *line == '>')
		return (syntax_error(">"));
	if ((c == '>' || c == '<') && *line == '<')
		return (syntax_error("<"));
	if (*line == ';')
		return (syntax_error(";"));
	if (*line == '|' && (c == ';' || c == '<'))
		return (syntax_error("|"));
	return (SUCCESS);
}

int	validate_line(char *line)
{
	int status;
	int i;

	status = SUCCESS;
	i = 0;
	if (check_begin(line) == FAILURE)
		return (FAILURE);
	while (line[i] != '\0' && status == SUCCESS)
	{
		if (line[i] == '\\')
			i++;
		else if (line[i] == ';')
			status = check_next_char(';', &line[i + 1]);
		i++;
	}
	if (status == FAILURE)
		return (FAILURE);
	status = check_end(line);
	return (status);
}

char	*read_line()
{
	char *line;
	int status;

	line = NULL;
	status = FAILURE;
	while (status != SUCCESS)
	{
		if (get_next_line(0, &line) == -1)
			return (NULL);
		if (line == NULL)
			continue;
		status = validate_line(line);
		if (status == FAILURE)
		{
			free_gc(line);
			line = NULL;
		}
	}
	return (line);
}