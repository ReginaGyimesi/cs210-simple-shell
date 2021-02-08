//
// Created by zkiss on 27/01/2021.
//

#include <main.h>

int main()
{
    int run_testing_methods = FALSE;
    if(run_testing_methods){
        testing(run_testing_methods);
    }
    else{
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
    }

    return 0;
}

void testing(int run_every_test){
    if(run_every_test){
        run_tests();
    }

}