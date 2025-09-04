#include "ft_popen.h"

int ft_popen(const char *file, char *const argv[], char type)
{
	pid_t pid;
	int fds[2];

	if (!file || !argv || (type != 'w' && type != 'r'))
		return -1;
	if (pipe(fds) < 0)
		return -1;
	pid = fork();
	if (pid < 0)
	{
		close(fds[0]);
		close(fds[1]);
		return -1;
	}
	if (pid == 0)
	{
		if (type == 'r')
			dup2(fds[1], STDOUT_FILENO);
		else
			dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
		close(fds[1]);
		execvp(file, argv);
		exit(1);
	}
	if (type == 'r')
	{
		close(fds[1]);
		return fds[0];
	}
	else
	{
		close(fds[0]);
		return fds[1];
	}
}

int main()
{
	int fd;
	char buffer[11];
	ssize_t nread;

	fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
	while (1)
	{
		nread = read(fd, buffer, sizeof(buffer) - 1);
		if (nread == -1)
			return 1;
		if (nread == 0)
			return 0;
		buffer[nread] = '\0';
		printf("%s", buffer);
	}

	// int fd;
	// char *line;

	// fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
	// if (fd < 0)
	// {
	// 	perror("ft_popen");
	// 	return 1;
	// }
	// while ((line = get_next_line(fd)))
	// 	write(1, line, strlen(line));
	return 0;
}

// to check the w
// int main()
// {
// 	int fd;
// 	const char *msg = "first line\nsecond line\nthird line\n";
// 	// запускаем wc -l, которая считает количество строк
// 	fd = ft_popen("cat", (char *const[]){"cat", NULL}, 'w');
// 	if (fd < 0)
// 	{
// 		perror("ft_popen");
// 		return 1;
// 	}
// 	write(fd, msg, strlen(msg));
// 	close(fd);        // нужно закрыть, чтобы wc получил EOF
// 	wait(NULL);       // ждём завершения дочернего процесса
// 	return 0;
// }