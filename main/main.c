//
// Created by Zoltan Kiss on 27/01/2021.
//

#include "main.h"

int main()
{
    char *line;
    char **args;
    int status;
    char *start_path = getenv("PATH"); // initial path stored
    char* history[20];
struct Alias**head;
    int front, rear;
    front = rear = -1;

    head=NULL;
    welcome();
    set_directory();
    make_history(history);
    load_history(history, &front, &rear);

    do {

        print_prompt();
        line = read_input();
        if (line != NULL && line[0] != '\0' && line[0] != '\n' && line[0] != '!')
            add_to_history(line, history, &front, &rear);

        args = tokenise(line);

        status = apply_command(args, history,&front, &rear,head);
        free(line);
        free(args);
    } while (status);

    save_history(history, &front, &rear);
    free_history(history); // later on, the history will be saved in a separate file
    setenv("PATH", start_path, 1); // initial path restored
    printf("Path is restored to %s\n",getenv("PATH"));
    
    return 0;
}
