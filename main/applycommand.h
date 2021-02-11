/*
// taking an array of tokenized inputs
// executing a command
// Created by Balázs on 2021. 02. 01..
*/

#include <unistd.h>

#ifndef CS210_SIMPLE_SHELL_APPLYCOMMAND_H
#define CS210_SIMPLE_SHELL_APPLYCOMMAND_H

#endif //CS210_SIMPLE_SHELL_APPLYCOMMAND_H

int apply_command(char** tokens) {
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
        char *inbuilt[] = {"cd", "exit"}; //an array where we store our non-linux commands
        // so far only exit but will grow later
        for (int i = 0; i < 2; ++i) {
            if (strcmp(tokens[0], "cd") == 0) //checking if the input is an inbuilt function and returning the
                return change_directory(tokens);
            if (strcmp(tokens[0], inbuilt[i]) == 0) //checking if the input is an inbuilt function and returning the
                return FALSE;                        // the index if it
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
