/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdahl <kdahl@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 21:54:35 by mtriston          #+#    #+#             */
/*   Updated: 2020/11/13 16:52:08 by kdahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		print_prompt(void)
{
	char	*path;
	char	*home;
	int		home_len;

	path = getcwd(NULL, 0);
	home = ft_getenv("HOME", g_env.env);
	home_len = ft_strlen(home);
	ft_putstr_fd("\033[1m \033[31m ", 0);
	ft_putstr_fd(ft_getenv("USER", g_env.env), 0);
	ft_putstr_fd(":", 0);
	if (ft_strncmp(path, home, home_len) == 0)
	{
		ft_putstr_fd("~", 0);
		ft_putstr_fd(path + home_len, 0);
	}
	else
		ft_putstr_fd(path, 0);
	ft_putstr_fd("$ \033[0m", 0);
	free(path);
}

void		shell_loop(void)
{
	char *cmd_line;

	while (1)
	{
		cmd_line = NULL;
		print_prompt();
		cmd_line = read_line();
		if (cmd_line != NULL)
		{
			execute_line(cmd_line);
			free_gc(cmd_line);
		}
	}
}

void		env_init(char **env)
{
	size_t		i;

	i = 0;
	g_env.status = 0;
	g_env.pid = 0;
	g_env.sigquit = 0;
	g_env.sigint = 0;
	g_env.env = malloc((envp_len(env) + 1) * sizeof(char *));
	if (!g_env.env)
		exit(EXIT_FAILURE);
	while (env[i])
	{
		g_env.env[i] = (char *)malloc((ft_strlen(env[i]) + 1) * sizeof(char));
		if (!g_env.env[i])
			exit(EXIT_FAILURE);
		ft_strcpy(g_env.env[i], env[i]);
		i++;
	}
	g_env.env[i] = NULL;
}

int			main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	signal(SIGINT, &signal_int);
	signal(SIGQUIT, &signal_quit);
	env_init(envp);
	shell_loop();
	return (0);
}
