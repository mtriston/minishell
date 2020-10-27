/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 22:16:56 by mtriston          #+#    #+#             */
/*   Updated: 2020/10/27 22:07:58 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parser.h"
#include "../lexer/lexer.h"



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

char 			*parse_next_cmd(char *cmd_line, t_cmd **cmd, char **env)
{
	t_token	*tokens;
	t_cmd	*current_cmd;
	char	*current_line;
	char	**splited_line;
	int		i;

	current_cmd = cmd_init(cmd);
	current_line = cmd_line;
	i = search_separator(cmd_line);
	if (cmd_line[i])
	{
		cmd_line[i] = '\0';
		i++;
	}
	splited_line = split_pipe(current_line);
	while (*splited_line)
	{
		tokens = lexer(*splited_line++, env);
		current_cmd->in = parse_redirect_in(&tokens, 0);
		current_cmd->out = parse_redirect_out(&tokens, 1);
		current_cmd->name = parse_cmd_name(&tokens);
		current_cmd->args = parse_cmd_args(&tokens);
		if (*splited_line)
			current_cmd = cmd_init(cmd);
	}
	tab_free_gc(splited_line);
	return (&cmd_line[i]);
}