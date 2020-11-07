/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 22:16:56 by mtriston          #+#    #+#             */
/*   Updated: 2020/11/07 16:20:51 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parser.h"
#include "../lexer/lexer.h"



static int		search_separator(const char *line)
{
	int i;
	int in_quote;
	int in_dquote;

	i = 0;
	in_quote = -1;
	in_dquote = -1;
	while (line[i])
	{
		if (line[i] == '\\')
			i++;
		else if (line[i] == '\'')
			in_quote *= -1;
		else if (line[i] == '\"')
			in_dquote *= -1;
		else if (line[i] == ';' && in_quote == -1 && in_dquote == -1)
			return (i);
		i++;
	}
	return (i);
}

int 		is_there_redirect(t_token *list, char token)
{
	while (list)
	{
		if (list->type == TYPE_SPECIAL && (list->data[0] == token))
			return (1);
		list = list->next;
	}
	return (0);
}

int 		parse_redirect_in(t_token **tokens, int fd)
{
	t_token	*ptr;

	ptr = *tokens;
	if (!is_there_redirect(*tokens, '<'))
		return (fd);
	while (ptr)
	{
		if (ptr->type == TYPE_SPECIAL && (ft_strcmp(ptr->data, "<") == 0))
		{
			if (ptr->next) {
				if (fd != 0)
					close(fd);
				if ((fd = open(ptr->next->data, O_RDONLY)) < 0)
				{
					ft_perror(ptr->next->data);
					return (-1);
				}
			}
			fd = fd > 0 ? fd : 0;
			remove_token(tokens, ptr->next);
			remove_token(tokens, ptr);
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
	if (!is_there_redirect(*tokens, '>'))
		return (fd);
	while (ptr)
	{
		if (ptr->type == TYPE_SPECIAL && (ptr->data[0] == '>'))
		{
			if (ptr->next) {
				if (fd != 1)
					close(fd);
				if (ptr->data[1] == '>')
					fd = open(ptr->next->data, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
				else
					fd = open(ptr->next->data, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
			}
			fd = fd > 1 ? fd : 1;
			remove_token(tokens, ptr->next);
			remove_token(tokens, ptr);
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
		//TODO: добавить поддержку >>
		if ((current_cmd->in = parse_redirect_in(&tokens, 0)) == -1)
		{
			g_env.status = 1;
			current_cmd->in = 0;
			continue;
		}
		current_cmd->out = parse_redirect_out(&tokens, 1);
		current_cmd->name = parse_cmd_name(&tokens);
		current_cmd->args = parse_cmd_args(&tokens);
		if (*splited_line)
			current_cmd = cmd_init(cmd);
	}
	tab_free_gc(splited_line);
	return (&cmd_line[i]);
}