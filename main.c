#include "minishell.h"

int	execute_commands(char **args)
{
	int		status;
	pid_t	pid;
	pid_t	wpid;

	pid = fork();
	if (pid == 0)
	{
		args[0] = ft_strjoin("/bin/", args[0]);
		if (execve(args[0], args, NULL) == -1)
			ft_putendl_fd("Command not found", 1);
		exit(EXIT_FAILURE);
	}
	else
	{
		wpid = waitpid(pid, &status, WUNTRACED);
		while (!WIFEXITED(status) && !WIFSIGNALED(status))
			wpid = waitpid(pid, &status, WUNTRACED);
	}
	return (1);

}
static void print_invite()
{
	char buf[200];

	getcwd(buf, 200);
	ft_putstr_fd("\033[1m \033[31m minishell:", 1);
	ft_putstr_fd(buf, 1);
	ft_putstr_fd("$ \033[0m", 1);

}
void		shell_loop(void)
{
	char	*line;
	char	**args;
	int		status;

	status = 1;
	while (status)
	{
		print_invite();
		get_next_line(0, &line);
		args = split_line(line);
		status = execute_commands(args);
		free_gc(line);
		line = NULL;
	}
	free_gc(line);
}

int	main(int argc, char **argv)
{
	shell_loop();

	return(1);
}
