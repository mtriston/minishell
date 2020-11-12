/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtriston <mtriston@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/22 19:21:06 by mtriston          #+#    #+#             */
/*   Updated: 2020/11/07 18:34:15 by mtriston         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# define TYPE_GENERAL 0
# define TYPE_SPECIAL 1
# define SYNTAX_ERROR 2
# define VALID_LINE 1
# define CONTINUE_INPUT 0

t_token		*lexer(char *line, char **env);
void		add_token_back(t_token **lst, t_token *new);
t_token		*token_init(int data_size, t_token **root);
void		token_del_one(t_token *lst, void (*del)(void*));
void		remove_token(t_token **root, t_token *for_delete);
int			token_list_size(t_token *lst);
int			validate_line(char *line);
char		*prepare_line(char *line, char **envp);

#endif
