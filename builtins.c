/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 13:58:22 by mtriston          #+#    #+#             */
/*   Updated: 2020/11/08 14:46:13 by mtriston         ###   ########.fr       */
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

	(void)envp;
	n_flag = 0;
	i = 0;
	if (cmd->args[0] != NULL)
	{
		if (ft_strncmp(cmd->args[1], "-n", 2) == 0)
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
		ft_perror("cd: too many arguments", 1);
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
		ft_perror("cd", 1);
		return (FAILURE);
	}
	//cmd_export("PWD", getcwd(NULL, 0), envp);
	return (SUCCESS);
}

int	cmd_env(t_cmd *cmd, char **envp)
{
	(void)cmd;
	while (*envp)
	{
		if (ft_strchr(*envp, '='))
			ft_putendl_fd(*envp, 1);
		envp++;
	}
	return (SUCCESS);
}

int	cmd_exit(t_cmd *cmd, char **envp)
{
	int status;
	int i;

	(void)envp;
	i = 0;
	status = 0;
	if (cmd && envp_len(cmd->args) > 2)
		return(ft_perror("too many arguments", 2));
	while (cmd && cmd->args[1] && cmd->args[1][i] && status == 0)
	{
		if (!ft_isdigit(cmd->args[1][i]))
			status = ft_perror("numeric argument requared", 1);
		i++;
	}
	if (cmd && cmd->args[1] && status == 0)
		status = ft_atoi(cmd->args[1]);
	free_gc(NULL);
	ft_free_array(g_env.env, free);
	ft_putendl_fd("exit", 2);
	exit(status);
}

int	cmd_pwd(t_cmd *cmd, char **envp)
{
	char buf[PATH_MAX];

	(void)cmd;
	(void)envp;
	getcwd(buf, PATH_MAX);
	ft_putendl_fd(buf, 1);
	return (SUCCESS);
}
