#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

int picoshell(char **cmds[])
{
	pid_t pid;
	int i;
	int exit_code;
	int fds[2];
	int prev_fd;
	int status;

	i = 0;
	exit_code = 0;
	prev_fd = -1;
	while (cmds[i])
	{
		if (cmds[i + 1])
		{
			if (pipe(fds) < 0)
				return 1;
		}
		pid = fork();
		if (pid < 0)
		{
			if (cmds[i + 1])
			{
				close(fds[0]);
				close(fds[1]);
			}
			if (prev_fd != -1)
				close(prev_fd);
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
				dup2(fds[1], STDOUT_FILENO);
				close(fds[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		else
		{

			if (prev_fd != -1)
				close(prev_fd);
			if (cmds[i + 1])
				close(fds[1]);
			prev_fd = fds[0];
			i++;
		}
	}
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			exit_code = 1;
		if (!WIFEXITED(status))
			exit_code = 1;
	}
	return exit_code;
}

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s cmd1 [args] '|' cmd2 [args] ...\n", argv[0]);
		return 1;
	}

	// Максимум количество команд = (argc + 1) / 2
	char ***cmds = malloc(sizeof(char **) * (argc / 2 + 2));
	if (!cmds)
	{
		perror("malloc");
		return 1;
	}

	int cmd_count = 0;
	int i = 1;
	while (i < argc)
	{
		// Пропускаем "|"
		if (strcmp(argv[i], "|") == 0)
		{
			i++;
			continue;
		}

		// Подсчитываем, сколько аргументов у текущей команды
		int j = i;
		while (j < argc && strcmp(argv[j], "|") != 0)
			j++;

		// Выделяем память под команду
		int arg_count = j - i;
		char **cmd = malloc(sizeof(char *) * (arg_count + 1));
		if (!cmd)
		{
			perror("malloc");
			return 1;
		}

		// Копируем аргументы
		for (int k = 0; k < arg_count; k++)
			cmd[k] = argv[i + k];
		cmd[arg_count] = NULL;

		cmds[cmd_count++] = cmd;

		i = j;
	}
	cmds[cmd_count] = NULL;

	// Запускаем пайплайн
	int ret = picoshell(cmds);

	// Освобождаем память
	for (int i = 0; i < cmd_count; i++)
		free(cmds[i]);
	free(cmds);

	return ret;
}

// ./picoshell2.o echo 'squalala' "|" cat "|" sed 's/a/b/g'