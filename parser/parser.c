/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 22:16:56 by mtriston          #+#    #+#             */
/*   Updated: 2020/10/27 20:48:21 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

static void		token_del_one(t_token *lst, void (*del)(void*))
{
	if (lst != NULL)
	{
		del(lst->data);
		free_gc(lst);
	}
}

static void		remove_token(t_token **root, t_token *for_delete)
{
	t_token	*node;
	t_token	*temp;
	int		i;

	if (*root && *root == for_delete)
	{
		temp = (*root)->next;
		token_del_one(*root, free_gc);
		(*root) = temp;
	}
	node = *root;
	temp = *root;
	i = 0;
	while (node != NULL)
	{
		if (node == for_delete)
		{
			temp->next = node->next;
			token_del_one(node, free_gc);
			node = temp;
			i = 0;
			break;
		}
		temp = i++ > 0 ? temp->next : temp;
		node = node != NULL ? node->next : node;
	}
}

int		token_list_size(t_token *lst)
{
	t_token	*ptr;
	int		i;

	if (!lst)
		return (0);
	ptr = lst;
	i = 1;
	while (ptr->next)
	{
		ptr = ptr->next;
		i++;
	}
	return (i);
}

static int		search_separator(const char *line)
{
	int i;

	i = 0;
	while (line[i] && line[i] != ';')
	{
		if (line[i] == '\\')
			i++;
		i++;
	}
	return (i);
}

int 		is_there_redirect(t_token *list, char *token)
{
	while (list)
	{
		if (list->type == TYPE_SPECIAL && (ft_strcmp(token, list->data) == 0))
			return (1);
		list = list->next;
	}
	return (0);
}

int 		parse_redirect_in(t_token **tokens, int fd)
{
	t_token	*ptr;

	ptr = *tokens;
	if (!is_there_redirect(*tokens, "<"))
		return (fd);
	while (ptr)
	{
		if (ptr->type == TYPE_SPECIAL && (ft_strcmp(ptr->data, "<") == 0))
		{
			if (ptr->next)
			{
				if (fd != 0)
					close(fd);
				fd = open(ptr->next->data, O_RDONLY);
			}
			fd = fd > 0 ? fd : 0;
			remove_token(tokens, ptr);
			remove_token(tokens, ptr->next);
			break;
		}
		ptr = ptr->next;
	}
	return (parse_redirect_in(tokens, fd));
}

int				parse_redirect_out(t_token **tokens, int fd)
{
	t_token	*ptr;

	ptr = *tokens;
	if (!is_there_redirect(*tokens, ">"))
		return (fd);
	while (ptr)
	{
		if (ptr->type == TYPE_SPECIAL && (ft_strcmp(ptr->data, ">") == 0))
		{
			if (ptr->next)
			{
				if (fd != 1)
					close(fd);
				fd = open(ptr->next->data, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
				printf("%d\n", fd);
			}
			fd = fd > 1 ? fd : 1;
			remove_token(tokens, ptr);
			remove_token(tokens, ptr->next);
			break;
		}
		ptr = ptr->next;
	}
	return (parse_redirect_out(tokens, fd));
}

static char		*parse_cmd_name(t_token **tokens)
{
	char *name;

	name = NULL;
	if (tokens && *tokens)
		name = ft_strdup((*tokens)->data);
	return (name);
}

static char 	**parse_cmd_args(t_token **tokens)
{
	t_token	*ptr;
	size_t	i;
	char 	**args;

	ptr = *tokens;
	i = 0;
	args = malloc_gc((token_list_size(*tokens) + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	while (ptr)
	{
		args[i++] = ptr->data;
		ptr = ptr->next;
	}
	args[i] = NULL;
	return (args);
}

char 			*parse_next_cmd(char *cmd_line, t_cmd *cmd, char **env)
{
	t_token	*tokens;
	char	*current_line;
	char	**splited_line;
	int		i;

	current_line = cmd_line;
	i = search_separator(cmd_line);
	if (cmd_line[i])
	{
		cmd_line[i] = '\0';
		i++;
	}
	splited_line = split_pipe(current_line);

	tokens = lexer(current_line, env);
	cmd->in = parse_redirect_in(&tokens, 0);
	cmd->out = parse_redirect_out(&tokens, 1);
	cmd->name = parse_cmd_name(&tokens);
	cmd->args = parse_cmd_args(&tokens);
	return (&cmd_line[i]);
}