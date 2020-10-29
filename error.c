/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/10 21:25:42 by mtriston          #+#    #+#             */
/*   Updated: 2020/10/29 19:28:10 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		syntax_error(char *token)
{
	ft_putstr_fd("minishell: ",1);
	ft_putstr_fd("syntax error near unexpected token `",1);
	ft_putstr_fd(token, 1);
	ft_putendl_fd("'", 1);
	return (2);
}

void	ft_perror(char *s)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putendl_fd(s, 2);
	if (errno != 0)
		ft_putendl_fd(strerror(errno), 2);
}