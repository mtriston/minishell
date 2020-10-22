/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/10 20:30:29 by mtriston          #+#    #+#             */
/*   Updated: 2020/10/22 23:25:16 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *find_command(char *cmd, char **envp)
{
	char **path_dirs;
	DIR *dir;
	struct dirent *entry;

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

int		launch_executable(t_cmd *cmd, char **envp)
{
	char path[PATH_MAX];
	pid_t pid;

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
	pid = fork();
	if (pid < 0)
		ft_perror("fork error");
	else if (pid == 0)
	{
		if (execve(path, cmd->args, envp) == -1)
			ft_perror(NULL);
		exit(EXIT_FAILURE);
	}
	else
		wait(NULL);
	//TODO: уведомление об ошибке?
	return (1);
}