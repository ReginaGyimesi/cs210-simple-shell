//
// Created by zkiss on 27/01/2021.
//

#include "main.h"

int main()
{
    char *line;
    char **args;
    int status;
    char* history[20];

    set_directory();
    make_history(history);
    welcome();
    
    do {

        print_prompt();
        line = read_input();

        args = tokenise(line);

        status = apply_command(args);

        free(line);
        free(args);
    } while (status);

    free_history(history); // later on, the history will be saved in a separate file
    return 0;
}
