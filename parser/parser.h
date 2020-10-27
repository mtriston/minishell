/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 21:10:49 by mtriston          #+#    #+#             */
/*   Updated: 2020/10/27 21:44:56 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
#define PARSER_H

char 		**split_pipe(char *line);
void		*tab_free_gc(char **tab);
t_cmd		*cmd_init(t_cmd **root);
#endif
