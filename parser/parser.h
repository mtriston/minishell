/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 21:10:49 by mtriston          #+#    #+#             */
/*   Updated: 2020/11/08 15:07:52 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

void		parse_redirects(t_cmd *cmd, t_token **tokens);
char		**split_pipe(char *line);
void		*tab_free_gc(char **tab);
t_cmd		*cmd_init(t_cmd **root);
int			search_separator(const char *line, char c);

#endif
