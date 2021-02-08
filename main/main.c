//
// Created by zkiss on 27/01/2021.
//

#include "main.h"

int main()
{
    char *line;
    char **args;
    int status;

    do {

        print_prompt();
        line = read_input();

        args = tokenise(line);

        status = apply_command(args);

        free(line);
        free(args);
    } while (status);

    return 0;
}