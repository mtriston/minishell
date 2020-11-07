/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 13:58:22 by mtriston          #+#    #+#             */
/*   Updated: 2020/11/07 18:11:03 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_getenv(char *arg, char **envp)
{
	if (ft_strcmp(arg, "?") == 0)
		return (ft_itoa(g_env.status));
	while (*envp)
	{
		if (ft_strncmp(*envp, arg, ft_found(*envp, '=')) == 0)
			return (*envp + ft_strlen(arg) + 1);
		envp++;
	}
	return ("");
}

int cmd_echo(t_cmd *cmd, char **envp)
{
	int		n_flag;
	size_t	i;

	n_flag = 0;
	i = 0;
	if (cmd->args[0] != NULL)
	{
		if (ft_strncmp(cmd->args[0], "-n", 2) == 0)
			n_flag = 1;
		while (cmd->args[++i])
		{
			ft_putstr_fd(cmd->args[i], 1);
			if (cmd->args[i + 1] != NULL)
				ft_putstr_fd(" ", 1);
		}
		if (!n_flag)
			ft_putstr_fd("\n", 1);
	}
		return(SUCCESS);
}

int cmd_cd(t_cmd *cmd, char **envp)
{
	char *dir;

	if (envp_len(cmd->args) > 2)
	{
		ft_perror("cd: too many arguments");
		return (FAILURE);
	}
	if (cmd->args[1] == NULL)
		dir = ft_getenv("HOME", envp);
	else if (ft_strcmp(cmd->args[1], "-") == 0)
	{
		dir = ft_getenv("HOME", envp);
		cmd_pwd(cmd, envp);
	}
	else
		dir = cmd->args[1];
	if (chdir(dir) != 0)
	{
		ft_perror("cd");
		return (FAILURE);
	}
	//cmd_export("PWD", getcwd(NULL, 0), envp);
	return (SUCCESS);
}

int	cmd_env(t_cmd *cmd, char **envp)
{
	while (*envp)
	{
		ft_putendl_fd(*envp, 1);
		envp++;
	}
	return (SUCCESS);
}

int	cmd_exit(t_cmd *cmd, char **envp)
{
	int status;

	if (cmd->args[1])
		status = ft_atoi(cmd->args[1]);
	else
		status = 0;
	free_gc(NULL);
	exit(status);
}

int	cmd_pwd(t_cmd *cmd, char **envp)
{
	char buf[PATH_MAX];

	getcwd(buf, PATH_MAX);
	ft_putendl_fd(buf, 1);
	return (SUCCESS);
}