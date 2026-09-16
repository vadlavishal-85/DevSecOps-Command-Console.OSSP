#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/builtin.h"

int execute_builtin(char **args)
{
    char cwd[1024];

    if (args[0] == NULL)
        return 1;

    /* exit */
    if (strcmp(args[0], "exit") == 0)
    {
        exit(EXIT_SUCCESS);
    }

    /* pwd */
    if (strcmp(args[0], "pwd") == 0)
    {
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s\n", cwd);

        return 1;
    }

    /* cd */
    if (strcmp(args[0], "cd") == 0)
    {
        if (args[1] == NULL)
        {
            printf("Usage: cd directory\n");
        }
        else
        {
            if (chdir(args[1]) != 0)
                perror("cd");
        }

        return 1;
    }

    /* clear */
    if (strcmp(args[0], "clear") == 0)
    {
        system("clear");
        return 1;
    }

    /* help */
    if (strcmp(args[0], "help") == 0)
    {
        printf("\nDevSecOps Command Console - Built-in Commands\n");
        printf("----------------------------------------------\n");
        printf("cd       Change directory\n");
        printf("pwd      Show current directory\n");
        printf("clear    Clear the terminal\n");
        printf("help     Show built-in commands\n");
        printf("env      Show environment variables\n");
        printf("exit     Exit the console\n");

        return 1;
    }

    /* env */
    if (strcmp(args[0], "env") == 0)
    {
        printf("HOME = %s\n", getenv("HOME"));
        printf("USER = %s\n", getenv("USER"));
        printf("PATH = %s\n", getenv("PATH"));

        return 1;
    }

    return 0;
}
