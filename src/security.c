#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../include/security.h"

/*
 * Commands allowed by the Week 7 security layer.
 * These are the commands currently implemented
 * in the DevSecOps Command Console.
 */
static const char *allowed_commands[] =
{
    "help",
    "exit",
    "cd",
    "pwd",
    "clear",
    "env",
    "ls",
    "echo",
    "cat",
    "mkdir",
    "touch",
    "cp",
    "mv",
    "rm",
    "sleep",
    NULL
};

int is_command_allowed(const char *command)
{
    int i = 0;

    while (allowed_commands[i] != NULL)
    {
        if (strcmp(command, allowed_commands[i]) == 0)
        {
            return 1;
        }

        i++;
    }

    return 0;
}

int validate_command(const char *command)
{
    if (command == NULL || strlen(command) == 0)
    {
        return 0;
    }

    /*
     * Reject shell metacharacters that could be used
     * for command chaining or redirection.
     */
    for (size_t i = 0; command[i] != '\0'; i++)
    {
        if (command[i] == ';' ||
            command[i] == '|' ||
            command[i] == '&' ||
            command[i] == '`' ||
            command[i] == '$')
        {
            return 0;
        }
    }

    return 1;
}
