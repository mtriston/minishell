/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdahl <kdahl@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 11:50:44 by kdahl             #+#    #+#             */
/*   Updated: 2020/11/08 14:22:25 by kdahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_int(int code)
{
	(void)code;
	if (g_env.pid == 0)
	{
		ft_putchar_fd('\n', 2);
		print_prompt();
	}
	else
	{
		kill(g_env.pid, g_env.status);
		ft_putstr_fd("\n", 2);
		g_env.status = 130;
	}
	g_env.sigint = 1;
}

void	signal_quit(int code)
{
	if (g_env.pid != 0)
	{
		kill(g_env.pid, g_env.status);
		ft_putstr_fd("Quit: ", 2);
		ft_putnbr_fd(code, 2);
		ft_putchar_fd('\n', 2);
		g_env.status = 131;
		g_env.sigquit = 1;
	}
	else
		ft_putstr_fd("\b\b  \b\b", 2);
}
