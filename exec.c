/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdahl <kdahl@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/10 20:30:29 by mtriston          #+#    #+#             */
/*   Updated: 2020/11/13 16:31:48 by kdahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		shell_notfound(t_cmd *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->name, 2);
	ft_putendl_fd(": command not found", 2);
}

static char		*find_command(char *cmd, char **envp)
{
	char			**path_dirs;
	DIR				*dir;
	struct dirent	*entry;

	if (!(path_dirs = ft_split(ft_getenv("PATH", envp), ":")))
		return (NULL);
	while (*path_dirs != NULL)
	{
		if (!(dir = opendir(*path_dirs)))
			return (NULL);
		while ((entry = readdir(dir)) != NULL)
		{
			if (ft_strcmp(entry->d_name, cmd) == 0)
				return (*path_dirs);
		}
		closedir(dir);
		path_dirs++;
	}
	return (NULL);
}

int				launch_executable(t_cmd *cmd, char **envp)
{
	char	path[PATH_MAX];

	ft_bzero(path, PATH_MAX);
	if (cmd->name[0] == '.' && cmd->name[1] == '/')
	{
		getcwd(path, PATH_MAX);
		ft_strlcat(path, cmd->name + 1, PATH_MAX);
	}
	else if (cmd->name[0] == '/')
		ft_strlcpy(path, cmd->name, PATH_MAX);
	else
	{
		ft_strlcpy(path, find_command(cmd->name, envp), PATH_MAX);
		ft_strlcat(path, "/", PATH_MAX);
		ft_strlcat(path, cmd->name, PATH_MAX);
	}
	execve(path, cmd->args, envp);
	if (cmd->name[0] == '/' || (cmd->name[0] == '.' && cmd->name[1] == '/'))
		ft_perror(cmd->name, 1);
	else
		shell_notfound(cmd);
	exit(127);
}
