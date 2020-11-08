/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 14:00:33 by mtriston          #+#    #+#             */
/*   Updated: 2020/11/08 14:29:21 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "lexer.h"

char	*read_line(void)
{
	char	*line;
	int		status;
	int		ret;

	line = NULL;
	status = CONTINUE_INPUT;
	while (status == CONTINUE_INPUT)
	{
		ret = get_next_line(0, &line);
		if (ret == -1)
			return (NULL);
		if (line == NULL)
			continue;
		if (ft_strlen(line) == 0 && ret == 0)
			cmd_exit(NULL, NULL);
		status = validate_line(line);
		if (status == SYNTAX_ERROR)
		{
			free_gc(line);
			line = NULL;
		}
	}
	return (line);
}
