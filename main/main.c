//
// Created by zkiss on 27/01/2021.
//

#include <stdio.h>
#include <main.h>

int main()
{
    testing(FALSE); //CHANGE THIS TO RUN TESTS OR NOT

    char *line;
    char **args;
    int status;

    do {

        print_prompt();
        line = read_input();

        printf("User input: ");
        printf("%s", line);
        printf("\n");

        args = tokenise(line);

        printf("Every argument:\n");
        for(int i = 0; args != NULL && args[i] != NULL; ++i){
            printf("%d. argument: ", i);
            printf("%s\n", args[i]);
        }

        status = apply_command(args);
        printf("Result status = %d\n", status);


    } while (status);

    return 0;
}

void testing(int run_every_test){
    if(run_every_test){
        run_tests();
    }

}