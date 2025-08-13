#include "ft_popen.h"

int	ft_popen(const char *file, char *const argv[], char type)
{
	int	fds[2];
	pid_t pid;

	if (!file || !argv || (type != 'r' && type != 'w'))
		return (-1);
	if (pipe(fds) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(fds[0]);
		close(fds[1]);
		return (-1);
	}
	if (pid == 0)
	{
		if (type == 'r')
		{
			if (dup2(fds[1], STDOUT_FILENO) == -1)
				exit(1);
		}
		else
		{
			if (dup2(fds[0], STDIN_FILENO) == -1)
				exit(1);
		}
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

// to check the r
int main()
{
	int fd;
	char *line;

	fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
	if (fd < 0)
	{
		perror("ft_popen");
		return 1;
	}
	while ((line = get_next_line(fd)))
		write(1, line, strlen(line));
	close(fd);
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


/* int	main() {
	int	fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
	dup2(fd, 0);
	fd = ft_popen("grep", (char *const []){"grep", "p", NULL}, 'r');
	char	*line;
	while ((line = get_next_line(fd)))
		printf("%s", line);
}
 */