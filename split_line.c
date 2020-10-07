#include "libft/libft.h"

static int	count_words(char const *s)
{
	int count;
	int i;

	count = 0;
	i = 0;
	while (s && s[i])
	{
		if (s[i] == '"')
		{
			count++;
			while (s[++i] != '"' && s[i])
				;
		}
		else if (s[i] == '\'')
		{
			count++;
			while (s[++i] != '\'' && s[i])
				;
		}
		else if (!ft_isspace(s[i]))
		{
			count++;
			while (s[i] != ' ' && s[i])
				i++;
		}
		i = s[i] ? i + 1 : i;
	}
	return (count);
}

static int	count_len(char const *s)
{
	int i;

	i = 0;
	if (s[i] == '"')
	{
		i++;
		while (s[i] != '"' && s[i])
			i++;
		i++;
	}
	else if (s[i] == '\'')
	{
		i++;
		while (s[i] != '\'' && s[i])
			i++;
		i++;
	}
	else
	{
		while (!ft_isspace(s[i]) && s[i])
			i++;
	}
	return (i);
}

static void	*ft_free_gc(char **tab)
{
	int i;

	i = 0;
	while (tab[i] != NULL)
		free_gc(tab[i++]);
	free_gc(tab);
	return (NULL);
}

char	**split_line(char const *s)
{
	int	i;
	int	j;
	int	word_count;
	char	*str;
	char	**array;

	str = (char *)s;
	i = 0;
	j = 0;
	word_count = count_words(s);
	if (!s || !(array = (char **)malloc_gc((word_count + 1) * sizeof(char *))))
		return (NULL);
	while (j < word_count)
	{
		i = count_len(str);
		if (i > 0)
		{
			if (!(array[j++] = ft_substr(str, 0, i)))
				return (ft_free_gc(array));
		}
		i++;
		str += i;
	}
	array[j] = NULL;
	return (array);
}