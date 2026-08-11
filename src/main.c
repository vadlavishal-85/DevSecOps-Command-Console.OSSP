
#include <stdio.h>
#include <string.h>

#define MAX_INPUT 1024

int main(){char input[MAX_INPUT];


printf(" Welcome to DevSecOps Command Console\n");

while (1)
{
    printf("devshell> ");

    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        break;
    }

    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "exit") == 0)
    {
        printf("Exiting DevSecOps Command Console...\n");
        break;
    }

    printf("You entered: %s\n", input);
}

return 0;

}
