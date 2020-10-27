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

#include "../minishell.h"
#include "lexer.h"

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