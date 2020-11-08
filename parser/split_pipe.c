/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 21:29:36 by mtriston          #+#    #+#             */
/*   Updated: 2020/10/29 21:06:47 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_words(char const *s)
{
	int count;
	int i;

	i = 0;
	count = 0;
	while (s && s[i])
	{
		if (s[i] != '|')
		{
			count++;
			while (s[i])
			{
				if (s[i] == '\\')
					i++;
				else if (s[i] == '|')
					break ;
				i++;
			}
		}
		i = s[i] ? i + 1 : i;
	}
	return (count);
}

static int	count_len(char const *str)
{
	int i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '\\')
			i++;
		else if (str[i] == '|')
			break ;
		i++;
	}
	return (i);
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
	if (!s || !(array = (char **)malloc_gc((word_count + 1) * sizeof(char *))))
		return (NULL);
	while (j < word_count)
	{
		i = count_len(str);
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
