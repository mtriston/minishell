#include "../../includes/minishell.h"
#include "../../includes/lexer.h"

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

void		token_del_one(t_token *lst, void (*del)(void*))
{
	if (lst != NULL)
	{
		del(lst->data);
		free_gc(lst);
	}
}

void		remove_token(t_token **root, t_token *for_delete)
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
			break ;
		}
		temp = i++ > 0 ? temp->next : temp;
		node = node != NULL ? node->next : node;
	}
}

int			token_list_size(t_token *lst)
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
