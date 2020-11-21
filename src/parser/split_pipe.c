#include "../../includes/minishell.h"
#include "../../includes/parser.h"

static int	count_words(char *s)
{
	int count;
	int i;

	i = 0;
	count = 0;
	while (s && *s)
	{
		i = search_separator(s, '|');
		s = s + i;
		if (*s)
			s++;
		count++;
	}
	return (count);
}

void		*tab_free_gc(char **tab)
{
	int i;

	i = 0;
	while (tab[i] != NULL)
		free_gc(tab[i++]);
	free_gc(tab);
	return (NULL);
}

char		**split_pipe(char *s)
{
	int		i;
	int		j;
	int		word_count;
	char	*str;
	char	**array;

	str = (char *)s;
	i = 0;
	j = 0;
	word_count = count_words(s);
	if (!s || !(array = (char **)calloc_gc((word_count + 1), sizeof(char *))))
		return (NULL);
	while (j < word_count)
	{
		i = search_separator(str, '|');
		if (i > 0)
		{
			if (!(array[j++] = ft_substr(str, 0, i)))
				return (tab_free_gc(array));
		}
		i++;
		str += i;
	}
	array[j] = NULL;
	return (array);
}
