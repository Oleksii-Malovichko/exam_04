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

void    ft_putstr(const char *s)
{
    if (!s) return;
    while (*s)
        write(1, s++, 1);
}

// int main()
// {
//     int  fd;
//     char *line;

//     // fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
// 	fd = ft_popen("grep", (char *const[]){"grep", "hello", NULL}, 'r');
//     while ((line = get_next_line(fd)))
//         ft_putstr(line);
//     return (0);
// }

int main(void)
{
    int fd1, fd2;
    char *line;

    printf("=== Test 1: simple ls ===\n");
    fd1 = ft_popen("ls", (char *const[]){"ls", "-l", NULL}, 'r');
    if (fd1 == -1)
    {
        perror("ft_popen(ls)");
        return (1);
    }
    while ((line = get_next_line(fd1)))
    {
        ft_putstr(line);
        free(line);
    }
    close(fd1);

    printf("\n=== Test 2: ls | grep \".c\" ===\n");
    // создаём пайпинг вручную через dup2
    fd1 = ft_popen("ls", (char *const[]){"ls", "-l", NULL}, 'r');
    if (fd1 == -1)
        return (1);

    // перенаправляем вывод ls → stdin текущего процесса
    dup2(fd1, STDIN_FILENO);
    close(fd1);

    // теперь читаем результат через grep
    fd2 = ft_popen("grep", (char *const[]){"grep", ".c", NULL}, 'r');
    if (fd2 == -1)
        return (1);

    while ((line = get_next_line(fd2)))
    {
        ft_putstr(line);
        free(line);
    }
    close(fd2);

    printf("\n=== Test 3: echo + wc ===\n");
    fd1 = ft_popen("echo", (char *const[]){"echo", "Hello world from ft_popen!", NULL}, 'r');
    dup2(fd1, STDIN_FILENO);
    close(fd1);

    fd2 = ft_popen("wc", (char *const[]){"wc", "-w", NULL}, 'r');
    while ((line = get_next_line(fd2)))
    {
        ft_putstr(line);
        free(line);
    }
    close(fd2);

    return (0);
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