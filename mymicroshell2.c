#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int tmp_stdin_fd;

int ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return i;
}

void	fatal()
{
	write(2, "error: fatal\n", 13);
	exit(1);
}

int print_error(char *str)
{
	write(2, str, ft_strlen(str));
	return 1;
}

int exec_cd(int argc, char **argv)
{
	if (argc != 2)
		return print_error("error: cd: bad arguments\n");
	if (chdir(argv[1]) == -1)
	{
		print_error("error: cd: cannot change directory to ");
		print_error(argv[1]);
		print_error("\n");
		exit(1);
	}
	return 0;
}

int exec_cmd(char **argv, int argc, char **envp, int is_pipe)
{
	int fd[2];
	int pid;

	if (is_pipe && pipe(fd) == -1)
		fatal();
	pid = fork();
	if (pid < 0)
		fatal();
	if (pid == 0)
	{
		argv[argc] = NULL;
		if (dup2(tmp_stdin_fd, 0) == -1)
			fatal();
		close(tmp_stdin_fd);
		if (is_pipe)
		{
			if (dup2(fd[1], 1) == -1)
				fatal();
			close(fd[0]);
			close(fd[1]);
		}
		execve(argv[0], argv, envp);
		print_error("error: cannot execute ");
		print_error(argv[0]);
		print_error("\n");
	}
	waitpid(pid, NULL, 0);
	close(tmp_stdin_fd);
	if (is_pipe)
	{
		close(fd[1]);
		tmp_stdin_fd = fd[0];
	}
	else
	{
		tmp_stdin_fd = dup(0);
		if (tmp_stdin_fd == -1)
			fatal();
	}
	return 0;
}

int main(int argc, char **argv, char **envp)
{
	int i = 1;
	int is_pipe;
	int start;

	tmp_stdin_fd = dup(0);
	if (tmp_stdin_fd == -1)
		fatal();
	while (i < argc)
	{
		start = i;
		is_pipe = 0;
		while (argv[i] && strcmp(argv[i], ";") != 0 && strcmp(argv[i], "|") != 0)
			i++;
		if (i < argc && strcmp(argv[i], "|") == 0)
			is_pipe = 1;
		if (i > start)
		{
			if (strcmp(argv[start], "cd") == 0)
				exec_cd(i - start, &argv[start]);
			else
				exec_cmd(&argv[start], i - start, envp, is_pipe);
		}
		i++;
	}
	return 0;
}