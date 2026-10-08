#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../include/process.h"
#include "../include/redirect.h"

int execute(char **tokens)
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid == 0)
    {
        /*
         * Week 9:
         * Handle redirection in the child process
         * before executing the command.
         */
        if (handle_redirection(tokens) == -1)
        {
            _exit(EXIT_FAILURE);
        }

        if (execvp(tokens[0], tokens) == -1)
        {
            perror("DevSecOps");
        }

        _exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("fork");
    }
    else
    {
        do
        {
            waitpid(pid, &status, WUNTRACED);
        }
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}
