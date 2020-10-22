/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/22 19:19:35 by mtriston          #+#    #+#             */
/*   Updated: 2020/10/22 19:42:39 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

void		add_token_back(t_token **lst, t_token *new)
{
	t_token *ptr;

	if (lst && *lst)
	{
		ptr = *lst;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
	}
	else if (lst)
		*lst = new;
}

t_token		*token_init(int data_size, t_token **root)
{
	t_token *elem;

	if (!(elem = malloc_gc(sizeof(t_token))))
		return (NULL);
	elem->data = calloc_gc((data_size + 1), sizeof(char));
	elem->type = TYPE_GENERAL;
	elem->next = NULL;
	add_token_back(root, elem);
	return (elem);
}

int 		is_there_env(const char *line)
{
	size_t i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\\')
			i++;
		else if (line[i] == '\'')
		{
			i++;
			while (line[i] && line[i] != '\'')
				i++;
		}
		else if (line[i] == '$')
			return (i);
		i++;
	}
	return (-1);
}