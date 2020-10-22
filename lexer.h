/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/22 19:21:06 by mtriston          #+#    #+#             */
/*   Updated: 2020/10/22 19:21:06 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
#define LEXER_H

#define TYPE_GENERAL 0
#define TYPE_SPECIAL 1

void		add_token_back(t_token **lst, t_token *new);
t_token		*token_init(int data_size, t_token **root);
int 		is_there_env(const char *line);

#endif
