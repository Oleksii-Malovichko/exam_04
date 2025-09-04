#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

int picoshell(char **cmds[])
{
	pid_t pid;
	int i;
	int exit_code;
	int status;
	int fds[2];
	int prev_fd;

	i = 0;
	exit_code = 0;
	prev_fd = -1;
	while (cmds[i])
	{
		if (cmds[i + 1] && pipe(fds) < 0)
			return 1;
		pid = fork();
		if (pid < 0)
		{
			if (cmds[i + 1])
			{
				close(fds[0]);
				close(fds[1]);
			}
			return 1;
		}
		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (cmds[i + 1])
			{
				close(fds[0]);
				dup2(fds[2], STDOUT_FILENO);
				close(fds[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (cmds[i + 1])
		{
			close(fds[1]);
			prev_fd = fds[0];
		}
		i++;
	}
	while (wait(&status) != -1)
	{
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			exit_code = 1;
	}
	return exit_code;
}

int main(void)
{
    char *echo[] = {"/bin/echo", "hello world", NULL};
    char *grep[] = {"/bin/grep", "world", NULL};
    char **cmds[] = {echo, grep, NULL};

    printf("=== TEST 2: echo | grep ===\n");
    int ret = picoshell(cmds);
    printf("Return code: %d\n\n", ret);
    return 0;
}