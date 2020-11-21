#include "../../includes/minishell.h"
#include "../../includes/lexer.h"

void	handle_redirect(char **line, t_token **token, int data_size)
{
	if ((*token)->data[0])
		*token = token_init(data_size, token);
	(*token)->type = TYPE_SPECIAL;
	(*token)->data[0] = **line;
	if (**line == '>' && **line == *((*line) + 1))
		(*token)->data[1] = *++*line;
	*token = token_init(data_size, token);
	*line += 1;
}

void	handle_blank(char **line, t_token **token, int data_size)
{
	size_t i;

	i = 0;
	while ((*line)[i] && ft_isblank((*line)[i]))
		i++;
	if ((*line)[i] && (ft_strlen((*token)->data) > 0))
		*token = token_init(data_size, token);
	*line += i;
}
