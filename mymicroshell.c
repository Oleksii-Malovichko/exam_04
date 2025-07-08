#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int tmp_stdin_fd;

int ft_strlen(char *s)
{
    int len = 0;
    while (s[len])
        len++;
    return len;
}

void fatal_error()
{
    write(2, "error: fatal\n", 13);
    exit(1);
}

int print_error(const char *msg)
{
    write(2, msg, ft_strlen((char *)msg));
    return 1;
}

int exec_cd(char **argv, int argc)
{
    if (argc != 2)
        return print_error("error: cd: bad arguments\n");
    if (chdir(argv[1]) == -1) {
        print_error("error: cd: cannot change directory to ");
        print_error(argv[1]);
        print_error("\n");
        return 1;
    }
    return 0;
}

int exec_command(char **argv, int argc, char **envp, int is_pipe)
{
    int fd[2];
    int pid;

    if (is_pipe && pipe(fd) == -1)
        fatal_error();

    pid = fork();
    if (pid < 0)
        fatal_error();

    if (pid == 0) // child
    {
        argv[argc] = NULL;

        if (dup2(tmp_stdin_fd, 0) == -1)
            fatal_error();
        close(tmp_stdin_fd);

        if (is_pipe)
        {
            if (dup2(fd[1], 1) == -1)
                fatal_error();
            close(fd[0]);
            close(fd[1]);
        }

        execve(argv[0], argv, envp);
        print_error("error: cannot execute ");
        print_error(argv[0]);
        print_error("\n");
        exit(1);
    }

    // parent
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
            fatal_error();
    }
    return 0;
}

int main(int argc, char **argv, char **envp)
{
    int i = 1;
    int start;
    int is_pipe;

    tmp_stdin_fd = dup(0);
    if (tmp_stdin_fd == -1)
        fatal_error();
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
                exec_cd(&argv[start], i - start);
            else
                exec_command(&argv[start], i - start, envp, is_pipe);
        }
        i++;
    }
    close(tmp_stdin_fd);
    return 0;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/wait.h>

// int tmp_stdin_fd;

// int ft_strlen(char *s)
// {
//     int len = 0;
//     while (s[len]) len++;
//     return len;
// }

// int print_error(const char *msg)
// {
//     write(2, msg, ft_strlen((char *)msg));
//     return 1;
// }

// int exec_cd(char **argv, int argc)
// {
//     if (argc != 2)
//         return print_error("error: cd: bad arguments\n");
//     if (chdir(argv[1]) == -1) {
//         print_error("error: cd: cannot change directory to ");
//         print_error(argv[1]);
//         print_error("\n");
//         return 1;
//     }
//     return 0;
// }

// int  exec_command(char **argv, int argc, char **envp, int is_pipe)
// {
//     int fd[2];
//     int pid;

//     if (is_pipe && pipe(fd) == -1)
//         return print_error("error: fatal\n");
//     pid = fork();
//     if (pid < 0)
//         return print_error("error: fatal\n");
//     if (pid == 0)
// 	{
//         argv[argc] = NULL;
//         if (dup2(tmp_stdin_fd, 0) == -1)
//             return print_error("error: fatal\n");
//         if (is_pipe && dup2(fd[1], 1) == -1)
//             return print_error("error: fatal\n");
//         close(tmp_stdin_fd);
//         if (is_pipe)
// 		{
//             close(fd[0]);
//             close(fd[1]);
//         }
//         execve(argv[0], argv, envp);
//         print_error("error: cannot execute ");
//         print_error(argv[0]);
//         print_error("\n");
//         exit(1);
//     }
//     waitpid(pid, NULL, 0);
//     close(tmp_stdin_fd);
//     if (is_pipe)
// 	{
//         close(fd[1]);
//         tmp_stdin_fd = fd[0];
//     }
// 	else
//         tmp_stdin_fd = dup(0);
//     return 0;
// }


// int main(int argc, char **argv, char **envp)
// {
// 	int i = 1;
// 	int start;
// 	int is_pipe;

// 	tmp_stdin_fd = dup(0);
// 	if (tmp_stdin_fd == -1)
// 		return print_error("error: fatal\n");
// 	while (i < argc)
// 	{
// 		start = i;
// 		is_pipe = 0;
// 		while (i < argc && strcmp(argv[i], ";") != 0 && strcmp(argv[i], "|") != 0)
// 			i++;
// 		if (i < argc && strcmp(argv[i], "|") == 0)
// 			is_pipe = 1;
// 		if (i > start)
// 		{
// 			if (strcmp(argv[start], "cd") == 0)
// 				exec_cd(&argv[start], i - start);
// 			else
// 				exec_command(&argv[start], i - start, envp, is_pipe);
// 		}
// 		i += 1;
// 	}
// 	close(tmp_stdin_fd);
// 	return 0;
// }