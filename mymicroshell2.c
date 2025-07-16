#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int tmp_stdin_fd;

int ft_strlen(char *str)
{
	int i = 0;
	while (str[i]) i++;
	return i;
}

int print_error(char *str)
{
	write(2, str, ft_strlen(str));
	return 1;
}

void	fatal()
{
	write(2, "error: fatal\n", 13);
	exit(1);
}

int exec_cd(int argc, char **argv)
{
	if (argc != 2)
		return print_error("error: cd: bad arguments\n");
	if (chdir(argv[1]) < 0)
	{
		print_error("error: cd: cannot change directory to ");
		print_error(argv[1]);
		print_error("\n");
		return 1;
	}
	return 0;
}

int exec_cmd(int argc, char **argv, char **envp, int is_pipe)
{
	int fd[2];
	int pid;

	if (is_pipe && pipe(fd) < 0)
		fatal();
	pid = fork();
	if (pid < 0)
		fatal();
	if (pid == 0)
	{
		argv[argc] = NULL;
		if (dup2(tmp_stdin_fd, 0) < 0)
			fatal();
		close(tmp_stdin_fd);
		if (is_pipe)
		{
			if (dup2(fd[1], 1) < 0)
				fatal();
			close(fd[0]);
			close(fd[1]);
		}
		execve(argv[0], argv, envp);
		print_error("error: cannot execute ");
		print_error(argv[0]);
		print_error("\n");
		return 1;
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
		if (tmp_stdin_fd < 0)
			fatal();
	}
	return 0;
}

int main(int argc, char **argv, char **envp)
{
	int start;
	int i = 1;
	int is_pipe;

	tmp_stdin_fd = dup(0);
	if (tmp_stdin_fd < 0)
		fatal();
	while (i < argc)
	{
		start = i;
		is_pipe = 0;
		while (i < argc && strcmp(argv[i], ";") != 0 && strcmp(argv[i], "|") != 0)
			i++;
		if (i < argc && strcmp(argv[i], "|") == 0)
			is_pipe = 1;
		if (i > start)
		{
			if (strcmp(argv[start], "cd") == 0)
				exec_cd(i - start, &argv[start]);
			else
				exec_cmd(i - start, &argv[start], envp, is_pipe);
		}
		i++;
	}
	close(tmp_stdin_fd);
	return 0;
}