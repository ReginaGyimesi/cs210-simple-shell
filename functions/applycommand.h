/*
// taking an array of tokenized inputs
// executing a command
// Created by Balázs on 2021. 02. 01..
*/



#ifndef CS210_SIMPLE_SHELL_APPLYCOMMAND_H
#define CS210_SIMPLE_SHELL_APPLYCOMMAND_H

#endif //CS210_SIMPLE_SHELL_APPLYCOMMAND_H

int apply_command(char** tokens) {
    if (tokens == NULL || tokens[0] == NULL) {   //Should return 0 to close the input feed and exit the loop
        return 0;
    }
    char *inbuilt[] = {"exit",}; //an array where we store our non-linux commands
    // so far only exit but will grow later
    for (int i = 0; i < 3; ++i) {
        if (strcmp(tokens[0], inbuilt[i]) == 0) //checking if the input is an inbuilt function and returning the
            return 0;                        // the index if it
    }
    return 1;       //here we will fork but currently returns 1 if its not null and not exit
}
