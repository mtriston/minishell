#include "libft/libft.h"

int	execute_commands(char **args)
{
	int i;

	i = 0;
	return (1);

}

void	shell_loop(void)
{
	char	*line;
	char	**args;
	int	status;

	get_next_line(0, &line);
	args = split_line(line);
	status = execute_commands(args);
}

int	main(int argc, char **argv)
{
	shell_loop();

	return(1);
}
