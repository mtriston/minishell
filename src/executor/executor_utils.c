#include "../../includes/minishell.h"

int			(*g_launch_builtin(int i))(t_cmd *cmd, char **envp)
{
	int (*g_launch_builtin[BUILTIN_NUM])(t_cmd *cmd, char **envp);

	g_launch_builtin[ECHO] = &cmd_echo;
	g_launch_builtin[CD] = &cmd_cd;
	g_launch_builtin[PWD] = &cmd_pwd;
	g_launch_builtin[EXPORT] = &cmd_export;
	g_launch_builtin[UNSET] = &cmd_unset;
	g_launch_builtin[ENV] = &cmd_env;
	g_launch_builtin[EXIT] = &cmd_exit;
	return (g_launch_builtin[i]);
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
