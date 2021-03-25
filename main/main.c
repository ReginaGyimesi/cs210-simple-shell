/**
 * CS210 Simple Shell - Group 5
 * Balazs Szalay, Eamonn McClay, Mark Oppo, Regina Gyimesi, Zoltan Kiss
 *
 * The main driver file
 */

#include "main.h"

int main()
{
    char *line;
    char **args;
    int status;
    char *start_path = getenv("PATH");      // Initial path stored

    set_directory();
    make_history();
    make_aliases();
    load_history();
    load_aliases();

    do {

        print_prompt();
        line = read_input();

        args = tokenise_string(line);

        status = apply_command(args);

        free(line);
    } while (status);

    save_aliases();
    save_history();
    free_history();
    free_aliases();
    setenv("PATH", start_path, 1); // initial path restored
    printf("Path is restored to %s\n",getenv("PATH"));

    return 0;
}