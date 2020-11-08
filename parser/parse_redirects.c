/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/08 15:00:55 by mtriston          #+#    #+#             */
/*   Updated: 2020/11/08 15:08:54 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parser.h"
#include "../lexer/lexer.h"

static int	is_there_redirect(t_token *list, char token)
{
	while (list)
	{
		if (list->type == TYPE_SPECIAL && (list->data[0] == token))
			return (1);
		list = list->next;
	}
	return (0);
}

static int	parse_redirect_out(t_token **tokens, int fd)
{
	t_token	*ptr;

	ptr = *tokens;
	if (!is_there_redirect(*tokens, '>'))
		return (fd);
	while (ptr)
	{
		if (ptr->type == TYPE_SPECIAL && (ptr->data[0] == '>'))
		{
			if (ptr->next)
			{
				if (fd != 1)
					close(fd);
				if (ptr->data[1] == '>')
					fd = open(ptr->next->data, 0100 | 01 | O_APPEND, S_IRWXU);
				else
					fd = open(ptr->next->data, O_CREAT | 01 | O_TRUNC, S_IRWXU);
			}
			remove_token(tokens, ptr->next);
			remove_token(tokens, ptr);
			break ;
		}
		ptr = ptr->next;
	}
	return (parse_redirect_out(tokens, fd));
}

static int	parse_redirect_in(t_token **tokens, int fd)
{
	t_token	*ptr;

	ptr = *tokens;
	if (!is_there_redirect(*tokens, '<'))
		return (fd);
	while (ptr)
	{
		if (ptr->type == TYPE_SPECIAL && (ft_strcmp(ptr->data, "<") == 0))
		{
			if (ptr->next)
			{
				if (fd != 0)
					close(fd);
				if ((fd = open(ptr->next->data, O_RDONLY)) < 0)
					return (ft_perror(ptr->next->data, -1));
			}
			remove_token(tokens, ptr->next);
			remove_token(tokens, ptr);
			break ;
		}
		ptr = ptr->next;
	}
	return (parse_redirect_in(tokens, fd));
}

void		parse_redirects(t_cmd *cmd, t_token **tokens)
{
	if ((cmd->in = parse_redirect_in(tokens, 0)) == -1)
	{
		g_env.status = 1;
		cmd->in = 0;
	}
	cmd->out = parse_redirect_out(tokens, 1);
}
