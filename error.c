/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/10 21:25:42 by mtriston          #+#    #+#             */
/*   Updated: 2020/10/10 21:31:01 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_perror(char *s)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putendl_fd(s, 2);
	if (errno != 0)
		ft_putendl_fd(strerror(errno), 2);
}