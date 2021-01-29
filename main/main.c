//
// Created by zkiss on 27/01/2021.
//

#include <stdio.h>
#include <main.h>

void sh_loop(void)
{
    char *line;
    char **args;
    int status;

    do {
        printf("> ");
        line = sh_read_line();
        args = sh_split_line(line);
        status = sh_execute(args);

        free(line);
        free(args);
    } while (status);
}

void testing(int run_every_test){
    if(run_every_test){
        run_tests();
    }

}