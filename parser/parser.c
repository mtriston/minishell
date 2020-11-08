/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 22:16:56 by mtriston          #+#    #+#             */
/*   Updated: 2020/11/08 15:05:10 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parser.h"
#include "../lexer/lexer.h"

static char	*parse_cmd_name(t_token **tokens)
{
	char *name;

	name = NULL;
	if (tokens && *tokens)
		name = ft_strdup((*tokens)->data);
	return (name);
}

static char	**parse_cmd_args(t_token **tokens)
{
	t_token	*ptr;
	size_t	i;
	char	**args;

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

char		*parse_next_cmd(char *cmd_line, t_cmd **cmd, char **env)
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
		cmd_line[i++] = '\0';
	splited_line = split_pipe(current_line);
	while (*splited_line)
	{
		tokens = lexer(*splited_line++, env);
		parse_redirects(current_cmd, &tokens);
		current_cmd->name = parse_cmd_name(&tokens);
		current_cmd->args = parse_cmd_args(&tokens);
		if (*splited_line)
			current_cmd = cmd_init(cmd);
	}
	tab_free_gc(splited_line);
	return (&cmd_line[i]);
}
