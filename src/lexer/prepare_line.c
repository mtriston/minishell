#include "../../includes/minishell.h"
#include "../../includes/lexer.h"

static int	is_there_spec(const char *line, char spec_symbol)
{
	int		in_dquote;
	size_t	i;

	i = 0;
	in_dquote = -1;
	while (line[i])
	{
		if (line[i] == '\\')
			i++;
		else if (line[i] == '"')
			in_dquote *= -1;
		else if (line[i] == '\'' && in_dquote == -1)
		{
			i++;
			while (line[i] && line[i] != '\'')
				i++;
		}
		else if (line[i] == spec_symbol)
			return (i);
		i++;
	}
	return (-1);
}

static char	*handle_env_var(char *line)
{
	char	*begin;
	char	*variable;
	char	*temp;
	size_t	i;

	i = 0;
	if (is_there_spec(line, '$') < 0)
		return (line);
	begin = line;
	line = line + is_there_spec(line, '$');
	*line = '\0';
	line++;
	if (ft_isdigit(line[i]) || line[i] == '?')
		i++;
	else
		while (line[i] && ft_isalnum(line[i]))
			i++;
	variable = ft_substr(line, 0, i);
	line += i;
	temp = ft_strjoin(begin, ft_getenv(variable, g_env.env));
	variable = temp;
	temp = ft_strjoin(variable, line);
	free_gc(variable);
	free_gc(line);
	return (handle_env_var(temp));
}

static char	*handle_tilda(char *line)
{
	char	*begin;
	char	*home_dir;
	char	*temp;

	if (is_there_spec(line, '~') < 0)
		return (line);
	begin = line;
	line = line + is_there_spec(line, '~');
	*line = '\0';
	line++;
	home_dir = ft_getenv("HOME", g_env.env);
	temp = ft_strjoin(begin, home_dir);
	home_dir = temp;
	temp = ft_strjoin(home_dir, line);
	free_gc(home_dir);
	free_gc(line);
	return (handle_tilda(temp));
}

char		*prepare_line(char *line)
{
	line = handle_env_var(line);
	line = handle_tilda(line);
	return (line);
}
