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

int				ft_found(const char *str, char c)
{
	int i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	if (str[i] == c)
		return (i);
	return (-1);
}

int				envp_len(char **envp)
{
	int len;

	len = -1;
	while (envp[++len])
		;
	return (len);
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
