/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdahl <kdahl@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 19:40:18 by kdahl             #+#    #+#             */
/*   Updated: 2020/10/24 22:42:44 by kdahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				ft_found(char *str, char c)
{
	int i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	if (str[i] == c)
		return (i);
	return (-1);
}

static void			free_it(char **str)
{
	int i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
	return ;
}

static void		export_no_arg(char **str)
{
	int i;

	i = 0;
	while (str[i])
	{
		write(1, "declare -x ", 11);
		write(1, str[i], ft_strlen(str[i]));
		write(1, "\n", 1);
		i++;
	}
	//free_it(envp);
}

int				envp_len(char **envp)
{
	int len;

	len = -1;
	while (envp[++len])
		;
	return (len);
}

static char		*new_arg(char *old)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = ft_calloc(ft_strlen(old) + 3, sizeof(char));
	while (old[i])
	{
		new[j++] = old[i++];
		if (old[i - 1] == '=')
		{
			if (!old[i])
				new[j++] = '"';
			new[j++] = '"';
		}
		else if (!old[i])
			new[j++] = '"';
	}
	new[j] = 0;
	return (new);
}

static char		**no_arg(char **envp)
{
	int i;
	int j;
	char *temp;

	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[j + 1])
		{
			if (ft_strcmp(envp[j], envp[j + 1]) > 0)
			{
				temp = envp[j];
				envp[j] = envp[j + 1];
				envp[j + 1] = temp;
			}
			j++;
		}
	i++;
	}
	return (envp);
}

static void		new_envp2(char *str, char **temp, int i, int j, char **envp)
{
	int sum;

	sum = 0;
	while (envp[++j])
		if (ft_strncmp(str, envp[j], ft_found(str, '=') + 1) == 0)
		{
			if (!(temp[j] = ft_strdup(str)))
				exit(1);
			sum = 1;
			break ;
		}
	if (!envp[j])
		if (!(temp[i] = ft_strdup(str)))
			exit(1);
	while (envp[i++])
	{
		if (!(temp[i - sum] = ft_strdup(envp[i - 1])))
			exit(1);
	}
	return ;
}

static void		new_envp(char *str, char **envp)
{
	char	**temp;
	char *temp2;
	char	**buff;
	int		count;
	int		i;
	int		j;
	int k;

	count = 2;
	i = -1;
	k = 0;
	while (envp[i])
	{
  		if (ft_strncmp(str, envp[i], ft_found(str, '=') + 1) == 0)
   		{
     		temp2 = envp[i];
      		envp[i] = str;
      		free_gc(temp2);
  	 	}
	}
	i = -1;
	j = -1;
	temp = (char**)ft_calloc(envp_len(envp) + count, sizeof(char*));
	while (envp[++i])
		temp[i] = envp[i];
	new_envp2(str, temp, i, j, envp);
	buff = envp;
	envp = temp;
	//free_it(buff);
}

void export_print_env(char **envp)
{
   int i;
   int j;
   char *temp;
   i = 0;
   while (envp[i])
   {
      j = 0;
      while (envp[j + 1])
      {
         if (ft_strcmp(envp[j], envp[j + 1]) > 0)
         {
            temp = envp[j];
            envp[j] = envp[j + 1];
            envp[j + 1] = temp;
         }
         j++;
      }
      i++;
   }
   while (*envp)
   {
      ft_putstr_fd("declare -x ", 1);
      ft_putendl_fd(*envp++, 1);
   }
}

int		sort_export(int i, int j, char **envp, t_cmd *cmd)
{
	char *temp;
	
	while (envp[j])
    {
        if (ft_strncmp(cmd->args[i], envp[j], ft_found(cmd->args[i], '=') + 1) == 0)
        {
            temp = envp[j];
            envp[j] = cmd->args[i];
            free_gc(temp);
            break ;
        }
        j++;
    }
	return (i);
}

int cmd_export(t_cmd *cmd, char **envp)
{
   int i;
   int j;
   int k;
   char **tab;

   i = 1;
   if (!(cmd->args[1]))
      export_print_env(envp);
   else
   {
         while (cmd->args[i])
         {
            j = 0;
			sort_export(i, j, envp, cmd);
            k = 0;
            j = 0;
            tab = calloc_gc(envp_len(envp) + 2, sizeof(char *));
            while (envp[k])
              tab[j++] = envp[k++];
            tab[j++] = cmd->args[i];
            tab[j] = NULL;
            envp = tab;
            i++;
       }
    }
    export_print_env(envp);
	return (1);
}