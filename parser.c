/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 22:16:56 by mtriston          #+#    #+#             */
/*   Updated: 2020/10/19 22:16:56 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static int		search_separator(char *line)
{
	int i;

	i = 0;
	while (line[i] && line[i] != ';' && line[i] != '|')
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

static char		*parse_cmd_name(t_list **tokens)
{
	if (tokens && *tokens)
		return ((*tokens)->content);
	return (NULL);
}

static char 	**parse_cmd_args(t_list **tokens)
{
	t_list	*ptr;
	size_t	i;
	char 	**args;

	ptr = *tokens;
	i = 0;
	args = malloc_gc((ft_lstsize(*tokens) + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	while (ptr)
	{
		args[i++] = ptr->content;
		ptr = ptr->next;
	}
	args[i] = NULL;
	return (args);
}

char 			*parse_next_cmd(char *cmd_line, t_cmd *cmd, char **env)
{
	t_list	*tokens;
	char	*current_line;
	int		i;

	current_line = cmd_line;
	i = search_separator(cmd_line);
	if (cmd_line[i])
	{
		cmd_line[i] = '\0';
		i++;
	}
	tokens = split_into_tokens(current_line);
	cmd->name = parse_cmd_name(&tokens);
	cmd->args = parse_cmd_args(&tokens);
//	cmd->in = parse_redirect_in(&tokens);
//	cmd->out = parse_redirect_out(&tokens);
	return (&cmd_line[i]);
}