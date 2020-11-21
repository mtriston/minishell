#include "../../includes/minishell.h"
#include "../../includes/lexer.h"

char		*read_line(void)
{
	char	*line;
	int		status;
	int		ret;

	line = NULL;
	status = CONTINUE_INPUT;
	while (status == CONTINUE_INPUT)
	{
		ret = get_next_line(0, &line);
		if (ret == -1)
			return (NULL);
		if (line == NULL)
			continue;
		if (ft_strlen(line) == 0 && ret == 0)
			cmd_exit(NULL, NULL);
		status = validate_line(line);
		if (status == SYNTAX_ERROR)
		{
			free_gc(line);
			line = NULL;
		}
	}
	return (line);
}
