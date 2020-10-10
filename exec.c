/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/10 20:30:29 by mtriston          #+#    #+#             */
/*   Updated: 2020/10/10 21:33:42 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *find_command(char *cmd, char **envp)
{
	char **path_dirs;
	DIR *dir;
	struct dirent *entry;

	if (!(path_dirs = ft_split(ft_getenv("PATH", envp), ':')))
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

int		launch_executable(char **args, char **envp)
{
	char path[PATH_MAX];
	pid_t pid;

	if (args[0][0] == '.' && args[0][1] == '/')
	{
		getcwd(path, PATH_MAX);
		ft_strlcat(path, args[0] + 1, PATH_MAX);
	}
	else if (args[0][0] == '/')
		ft_strlcpy(path, args[0], PATH_MAX);
	else
	{
		ft_strlcpy(path, find_command(args[0], envp), PATH_MAX);
		ft_strlcat(path, "/", PATH_MAX);
		ft_strlcat(path, args[0], PATH_MAX);
	}
	pid = fork();
	if (pid < 0)
		ft_perror("fork error");
	else if (pid == 0)
	{
		if (execve(path, args, envp) == -1)
		{
			ft_perror("minishell");
			exit(EXIT_FAILURE);
		}
	}
	else
		wait(NULL);
	//TODO: уведомление об ошибке?
	return (1);
}