/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 14:00:33 by mtriston          #+#    #+#             */
/*   Updated: 2020/10/29 19:08:02 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "lexer.h"

char	*read_line()
{
	char *line;
	int status;

	line = NULL;
	status = CONTINUE_INPUT;
	while (status == CONTINUE_INPUT)
	{
		if (get_next_line(0, &line) == -1)
			return (NULL);
		if (line == NULL)
			continue;
		status = validate_line(line);
		if (status == SYNTAX_ERROR)
		{
			free_gc(line);
			line = NULL;
		}
	}
	return (line);
}