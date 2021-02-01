/*
// taking an array of tokenized inputs
// executing if its a valid command
// Created by Balázs on 2021. 02. 01..
*/

#ifndef CS210_SIMPLE_SHELL_APPLYCOMMAND_H
#define CS210_SIMPLE_SHELL_APPLYCOMMAND_H

#endif //CS210_SIMPLE_SHELL_APPLYCOMMAND_H
int apply_command(char** tokens){
    int i=0;
    if(tokens[0]==NULL) //if the input is null then nothing to apply hence return 1
    {
        return 1;
    }
    switch (tokens[0]) //if not see if its one of our own functions so far only exit
    {
        case "exit"||EOF:
            return 0;

    }

}