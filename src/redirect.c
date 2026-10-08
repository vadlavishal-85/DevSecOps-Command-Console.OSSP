#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include/redirect.h"

int handle_redirection(char **args)
{
    int i = 0;
    int fd;

    while (args[i] != NULL)
    {
        /* Output redirection: > */
        if (strcmp(args[i], ">") == 0)
        {
            if (args[i + 1] == NULL)
            {
                fprintf(stderr, "Redirection error: missing output file\n");
                return -1;
            }

            fd = open(args[i + 1],
                      O_WRONLY | O_CREAT | O_TRUNC,
                      0644);

            if (fd < 0)
            {
                perror("open");
                return -1;
            }

            if (dup2(fd, STDOUT_FILENO) < 0)
            {
                perror("dup2");
                close(fd);
                return -1;
            }

            close(fd);
            args[i] = NULL;
            return 0;
        }

        /* Append redirection: >> */
        if (strcmp(args[i], ">>") == 0)
        {
            if (args[i + 1] == NULL)
            {
                fprintf(stderr, "Redirection error: missing output file\n");
                return -1;
            }

            fd = open(args[i + 1],
                      O_WRONLY | O_CREAT | O_APPEND,
                      0644);

            if (fd < 0)
            {
                perror("open");
                return -1;
            }

            if (dup2(fd, STDOUT_FILENO) < 0)
            {
                perror("dup2");
                close(fd);
                return -1;
            }

            close(fd);
            args[i] = NULL;
            return 0;
        }

        /* Input redirection: < */
        if (strcmp(args[i], "<") == 0)
        {
            if (args[i + 1] == NULL)
            {
                fprintf(stderr, "Redirection error: missing input file\n");
                return -1;
            }

            fd = open(args[i + 1], O_RDONLY);

            if (fd < 0)
            {
                perror("open");
                return -1;
            }

            if (dup2(fd, STDIN_FILENO) < 0)
            {
                perror("dup2");
                close(fd);
                return -1;
            }

            close(fd);
            args[i] = NULL;
            return 0;
        }

        /* Error redirection: 2> */
        if (strcmp(args[i], "2>") == 0)
        {
            if (args[i + 1] == NULL)
            {
                fprintf(stderr,
                        "Redirection error: missing error file\n");
                return -1;
            }

            fd = open(args[i + 1],
                      O_WRONLY | O_CREAT | O_TRUNC,
                      0644);

            if (fd < 0)
            {
                perror("open");
                return -1;
            }

            if (dup2(fd, STDERR_FILENO) < 0)
            {
                perror("dup2");
                close(fd);
                return -1;
            }

            close(fd);
            args[i] = NULL;
            return 0;
        }

        i++;
    }

    return 0;
}
