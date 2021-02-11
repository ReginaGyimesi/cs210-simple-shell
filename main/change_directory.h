//
// Created by zkiss on 09/02/2021.
//

#ifndef CS210_SIMPLE_SHELL_CHANGE_DIRECTORY_H
#define CS210_SIMPLE_SHELL_CHANGE_DIRECTORY_H

#endif //CS210_SIMPLE_SHELL_CHANGE_DIRECTORY_H

int change_directory(char** tokens){
    if(tokens == NULL || *tokens == NULL || tokens[0] == NULL){
        perror("NULL pointer error in cd command\n");
        return ERROR;
    }

    if(tokens[2] != NULL){
        fprintf(stderr, "Too many arguments. Use case: cd [path/to/directory]\n");
        return ERROR;
    }

    if(tokens[1] == NULL){ // no parameter were given, set to user's home directory
        if(chdir(getenv("HOME"))!=0){
            perror("Failed to change HOME, maybe it does not exist?");
            return ERROR;
        }
        return TRUE;
    }
    else{               // some parameter were given, execute cd with that param
        if(chdir(tokens[1]) != 0){  // disregarding other params, only using the next param after cd
            char* s = malloc(sizeof(char*)*100);
            strcpy(s, "Failed to change to ");
            strcat(s, tokens[1]);
            strcat(s, " Use case: cd [path/to/directory]");
            perror(s);
            free(s);
            return ERROR;
        }
    }

    return TRUE;

}