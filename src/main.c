#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/shell.h"
#include "../include/input.h"
#include "../include/parser.h"
#include "../include/process.h"
#include "../include/builtin.h"
#include "../include/signals.h"
#include "../include/security.h"
#include "../include/thread.h"

int main()
{
    char *line;
    char **tokens;

    initialize_signals();
    start_monitor_thread();

    printf("=================================\n");
    printf("%s Version %s\n", SHELL_NAME, VERSION);
    printf("=================================\n");

    while (1)
    {
        printf("devshell> ");

        line = read_line();

        if (strlen(line) == 0)
        {
            free(line);
            continue;
        }

        /*
         * Week 7 Security Layer:
         * Validate the complete user input
         * before parsing or execution.
         */
        if (!validate_command(line))
        {
            printf("Security Alert: Invalid command input blocked.\n");
            free(line);
            continue;
        }

        tokens = parse_line(line);

        if (tokens == NULL || tokens[0] == NULL)
        {
            free_tokens(tokens);
            free(line);
            continue;
        }

        /*
         * Check whether the command is present
         * in the security whitelist.
         */
        if (!is_command_allowed(tokens[0]))
        {
            printf("Security Alert: Command '%s' is not allowed.\n",
                   tokens[0]);

            free_tokens(tokens);
            free(line);
            continue;
        }

       int builtin_status = execute_builtin(tokens);

if (builtin_status == 0)
{
    execute(tokens);
}

free_tokens(tokens);
free(line);

if (builtin_status == 2)
{
    break;
}
    }

    return 0;
}
