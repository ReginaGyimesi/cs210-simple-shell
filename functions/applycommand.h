/*
// taking an array of tokenized inputs
// executing a command
// Created by Balázs on 2021. 02. 01..
*/



#ifndef CS210_SIMPLE_SHELL_APPLYCOMMAND_H
#define CS210_SIMPLE_SHELL_APPLYCOMMAND_H

#endif //CS210_SIMPLE_SHELL_APPLYCOMMAND_H

int apply_command(char** tokens) {
    if (tokens[0] == NULL) {   //if the first input is null we just return -1 cuz its gonna be an error
        return -1;
    }
    char *inbuilt[] = {"exit"}; //an array where we store our non-linux commands
    // so far only exit but will grow later
    for (int i = 0; i < 1; ++i) {
        if (strcmp(tokens[0], inbuilt[i]) == 0) //checking if the input is an inbuilt function and returning the
            return i;                        // the index if it
    }
    return 1;       //here we will fork but currently returns 1 if its not null and not exit
}