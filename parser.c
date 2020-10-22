/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 22:16:56 by mtriston          #+#    #+#             */
/*   Updated: 2020/10/22 22:59:01 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static t_list	*split_into_tokens(char *line)
{
	t_list	*tokens;
	char 	*data;
	int		i;
	int 	j;

	tokens = NULL;
	data = calloc_gc(ft_strlen(line) + 1, sizeof(char));
	i = 0;
	j = 0;
	while (line[i] != '\0')
	{
		if (ft_isalnum(line[i]))
			data[j++] = line[i++];
		if (line[i] == '\\')
		{
			i++;
			data[j++] = line[i++];
		}
		if (!ft_isspace(line[i]))
			data[j++] = line[i++];
		if (ft_isspace(line[i]))
		{
			while (ft_isspace(line[i]))
				i++;
			ft_lstadd_back(&tokens, ft_lstnew(data));
			data = calloc_gc(sizeof(data), sizeof(char));
			j = 0;
		}
	}
	ft_lstadd_back(&tokens, ft_lstnew(data));
	return (tokens);
}

/*
int 		parse_redirect_in(t_list **tokens)
{
	t_list	*ptr;
	int 	fd;

	ptr = *tokens;
	fd = 0;
	while (ptr)
	{
		if (ft_strcmp(ptr->content, "<"))
		{
			free_gc(ptr->content);
			ptr->content = NULL;
			ptr = ptr->next;
			if (!(fd = open(ptr ? ptr->content : NULL, O_RDONLY)))
				ft_perror("");
			}
			if (ptr)
			{
				free(ptr->content);
				ptr->content = NULL;
			}
		}
		ptr = ptr->next;
	}
}
*/

static char		*parse_cmd_name(t_token **tokens)
{
	if (tokens && *tokens)
		return ((*tokens)->data);
	return (NULL);
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
	int		i;

	current_line = cmd_line;
	i = search_separator(cmd_line);
	if (cmd_line[i])
	{
		cmd_line[i] = '\0';
		i++;
	}
	tokens = lexer(current_line, env);
	cmd->name = parse_cmd_name(&tokens);
	cmd->args = parse_cmd_args(&tokens);
//	cmd->in = parse_redirect_in(&tokens);
//	cmd->out = parse_redirect_out(&tokens);
	return (&cmd_line[i]);
}