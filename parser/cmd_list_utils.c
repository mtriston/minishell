/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 21:51:00 by mtriston          #+#    #+#             */
/*   Updated: 2020/10/27 21:51:00 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void		add_cmd_back(t_cmd **lst, t_cmd *new)
{
	t_cmd *ptr;

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

t_cmd			*cmd_init(t_cmd **root)
{
	t_cmd *elem;

	if (!(elem = malloc_gc(sizeof(t_cmd))))
		return (NULL);
	elem->name = NULL;
	elem->args = NULL;
	elem->in = 0;
	elem->out = 1;
	elem->prev = NULL;
	elem->next = NULL;
	add_cmd_back(root, elem);
	return (elem);
}