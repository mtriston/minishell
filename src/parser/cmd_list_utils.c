#include "../../includes/minishell.h"
#include "../../includes/lexer.h"

static void	add_cmd_back(t_cmd **lst, t_cmd *new)
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

t_cmd		*cmd_init(t_cmd **root)
{
	t_cmd *elem;

	if (!(elem = malloc_gc(sizeof(t_cmd))))
		return (NULL);
	elem->name = NULL;
	elem->args = NULL;
	elem->in = 0;
	elem->out = 1;
	elem->next = NULL;
	add_cmd_back(root, elem);
	return (elem);
}

int			search_separator(const char *line, char c)
{
	int i;
	int in_quote;
	int in_dquote;

	i = 0;
	in_quote = -1;
	in_dquote = -1;
	while (line[i])
	{
		if (line[i] == '\\' && in_quote == -1 && in_dquote == -1)
			i++;
		else if (line[i] == '\'' && in_dquote == -1)
			in_quote *= -1;
		else if (line[i] == '\"' && in_quote == -1)
			in_dquote *= -1;
		else if (line[i] == c && in_quote == -1 && in_dquote == -1)
			return (i);
		i++;
	}
	return (i);
}
