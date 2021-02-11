/*
// taking an array of tokenized inputs
// executing a command
// Created by Balázs on 2021. 02. 01..
*/

#include <unistd.h>
#include <stdio.h>

#ifndef CS210_SIMPLE_SHELL_APPLYCOMMAND_H
#define CS210_SIMPLE_SHELL_APPLYCOMMAND_H
#define COMMANDS_LENGTH 4

#endif //CS210_SIMPLE_SHELL_APPLYCOMMAND_H







int apply_command(char** tokens) {
    char *builtin_str[] = {
            "exit",
            "getpath",
            "setpath",
            "cd"
    };
    int (*builtin_func[]) (char **) = {
            &exit1,
            &getpath,
            &setpath,
            &change_directory
    };

    if (tokens == NULL){   //if the first input is null or a NULL char, then we return 0 thus indicating it is an exit
        return FALSE;
    }

    else if(*tokens == NULL){ //this happens if the user only pressed Enter, and nothing else as input
        return ERROR;
    }

    else if(*tokens[0] == '\0'){ //if the first input is null or a NULL char, then we return 0 thus indicating it is an exit
        return FALSE;
    }
    else
    {

        // so far only exit but will grow later
        for (int i = 0; i < COMMANDS_LENGTH; ++i) {
            if (strcmp(tokens[0], builtin_str[i]) == 0) //checking if the input is an inbuilt function and returning the
                return (*builtin_func[i])(tokens);                        // the index if it
        }
        pid_t pid;
        pid=fork();
        if(pid<0)
        {
            printf("Fork failed");
            return TRUE;
        }
        else if(pid==0)
        {
            execvp(tokens[0],tokens);
            perror("Error: ");
            exit(EXIT_FAILURE);
            return TRUE;
        }
        else{
            wait(NULL);
            printf("Child complete\n");
            return TRUE;
        }

    }

}