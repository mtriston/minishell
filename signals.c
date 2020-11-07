/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdahl <kdahl@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 11:50:44 by kdahl             #+#    #+#             */
/*   Updated: 2020/11/07 14:33:02 by kdahl            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		*memfree(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
	return (NULL);
}

void	signal_int(int code, t_sig *sig)
{
	(void)code;
	if (sig->pid == 0)
	{
		ft_putstr_fd("\b\b  ", 2);
		ft_putstr_fd("\n", 2);
		ft_putstr_fd("💀 minishell ", 2);
	}
	else
	{
		ft_putstr_fd("\n", 2);
		sig->status = 130;
	}
	sig->sigint = 1;
}

void	signal_quit(int code, t_sig *sig)
{
	char	*nbr;

	nbr = ft_itoa(code);
	if (sig->pid != 0)
	{
		ft_putstr_fd("Quit: ", 2);
		ft_putendl_fd(nbr, 2);
		sig->status = 131;
		sig->sigquit = 1;
	}
	else
		ft_putstr_fd("\b\b  \b\b", 2);
	memfree(nbr);
}

t_sig	signal_init(void) //void?
{
	t_sig	signal;
	
	signal.sigint = 0;
	signal.sigquit = 0;
	signal.pid = 0;
	signal.status = 0;
	return (signal);
}