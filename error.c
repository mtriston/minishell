/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/10 21:25:42 by mtriston          #+#    #+#             */
/*   Updated: 2020/11/07 18:59:05 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		syntax_error(char *token)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("syntax error near unexpected token `", 2);
	ft_putstr_fd(token,  2);
	ft_putendl_fd("'",  2);
	return (2);
}

int ft_perror(char *s, int code)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(s, 2);
	if (errno != 0)
	{
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
	}
	else
		write(1, "\n", 2);
	return (code);
}
