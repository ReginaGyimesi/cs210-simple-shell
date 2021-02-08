/*
// taking an array of tokenized inputs
// executing a command
// Created by Balázs on 2021. 02. 01..
*/



#ifndef CS210_SIMPLE_SHELL_APPLYCOMMAND_H
#define CS210_SIMPLE_SHELL_APPLYCOMMAND_H

#endif //CS210_SIMPLE_SHELL_APPLYCOMMAND_H

int apply_command(char** tokens) {
    if (tokens == NULL){   //if the first input is null or a NULL char, then we return 0 thus indicating it is an exit
        return 0;
    }

    if(*tokens == NULL){ //this happens if the user only pressed Enter, and nothing else as input
        return -1;
    }

    if(*tokens[0] == '\0'){ //if the first input is null or a NULL char, then we return 0 thus indicating it is an exit
        return 0;
    }

    char *inbuilt[] = {"exit"}; //an array where we store our non-linux commands
    // so far only exit but will grow later
    for (int i = 0; i < 1; ++i) {
        if (strcmp(tokens[0], inbuilt[i]) == 0) //checking if the input is an inbuilt function and returning the
            return 0;                        // the index if it
    }

    return 1;       //here we will fork but currently returns 1 if its not null and not exit
}
