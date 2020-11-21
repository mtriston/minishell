#ifndef LEXER_H
# define LEXER_H

# define TYPE_GENERAL 0
# define TYPE_SPECIAL 1
# define SYNTAX_ERROR 2
# define VALID_LINE 1
# define CONTINUE_INPUT 0

void		handle_blank(char **line, t_token **token, int data_size);
void		handle_redirect(char **line, t_token **token, int data_size);
t_token		*lexer(char *line);
void		add_token_back(t_token **lst, t_token *new);
t_token		*token_init(int data_size, t_token **root);
void		token_del_one(t_token *lst, void (*del)(void*));
void		remove_token(t_token **root, t_token *for_delete);
int			token_list_size(t_token *lst);
int			validate_line(char *line);
char		*prepare_line(char *line);

#endif
