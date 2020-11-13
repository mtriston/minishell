/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launching_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdahl <kdahl@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 16:51:49 by kdahl             #+#    #+#             */
/*   Updated: 2020/11/13 16:52:10 by kdahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			(*launch_builtin(int i))(t_cmd *cmd, char **envp)
{
	int (*launch_builtin[BUILTIN_NUM])(t_cmd *cmd, char **envp);

	launch_builtin[ECHO] = &cmd_echo;
	launch_builtin[CD] = &cmd_cd;
	launch_builtin[PWD] = &cmd_pwd;
	launch_builtin[EXPORT] = &cmd_export;
	launch_builtin[UNSET] = &cmd_unset;
	launch_builtin[ENV] = &cmd_env;
	launch_builtin[EXIT] = &cmd_exit;
	return (launch_builtin[i]);
}

void		wait_child(pid_t pid)
{
	pid_t		wpid;

	while (1)
	{
		wpid = waitpid(pid, &g_env.status, WUNTRACED);
		if (wpid >= 0)
			break ;
	}
	if (WIFEXITED(g_env.status))
		g_env.status = WEXITSTATUS(g_env.status);
	if (g_env.sigint)
	{
		g_env.status = 130;
		g_env.sigint = 0;
	}
	if (g_env.sigquit)
	{
		g_env.status = 131;
		g_env.sigquit = 0;
	}
}

void		destroy_cmd(t_cmd **lst)
{
	t_cmd	*ptr;
	t_cmd	*temp;

	if (lst && *lst)
	{
		ptr = *lst;
		while (ptr)
		{
			temp = ptr;
			ptr = ptr->next;
			free_gc(temp->name);
			free_gc(temp->next);
			ft_free_array(temp->args, free_gc);
			free_gc(temp);
		}
		*lst = NULL;
	}
}
