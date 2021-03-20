//
// Created by Balazs Szalay on 2021. 02. 11.
//
// header for our inbuilt,non-linux commands:cd,exit,getpath,setpath
// further dev: Create your inbuilt function somewhere here, then add it in the apply command.h
// you have to add its keyword to the inbuilt array AND you have to store its pointer in the function
// pointers array at the corresponding index
//
//

#ifndef CS210_SIMPLE_SHELL_INBUILT_H
#define CS210_SIMPLE_SHELL_INBUILT_H

#endif //CS210_SIMPLE_SHELL_INBUILT_H

/*
 * Function to change directory
 */
int change_directory(char** tokens, char** history __attribute__((unused)), const int* front __attribute__((unused)), const int* rear __attribute__((unused))) {

    // initial error checks
    if (tokens == NULL || *tokens == NULL || tokens[0] == NULL) {
        perror("NULL pointer error in cd command\n");
        return ERROR;
    }

    if (tokens[1] == NULL) { // no parameter were given, set to user's home directory
        if (chdir(getenv("HOME")) != 0) {
            printf("%s", getenv("HOME"));
            perror("Failed to change HOME, maybe it does not exist?");

            free_tokens(tokens);

            return ERROR;
        }
        printf("Changed directory to HOME\n");

        free_tokens(tokens);

        return TRUE;
    }
    
    if (tokens[2] != NULL) {
        fprintf(stderr, "Too many arguments. Use case: cd [path/to/directory]\n");

        free_tokens(tokens);

        return ERROR;
    }
    
    else {               // some parameter were given, execute cd with that param
        if (chdir(tokens[1]) != 0) {  // disregarding other params, only using the next param after cd
            char *s = malloc(sizeof(char *) * 100);
            strcpy(s, "Failed to change to ");
            strcat(s, tokens[1]);
            strcat(s, ". Use case: cd [path/to/directory]");
            perror(s);
            free(s);

            free_tokens(tokens);

            return ERROR;
        }
        else{
            // checks if current directory is set to home
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) == NULL) {
                perror("getcwd() error");

                free_tokens(tokens);

                return ERROR;
            }

            char* new_dir = malloc(sizeof(char*)*PATH_MAX);
            strcpy(new_dir, "Changed directory to ");
            strcat(new_dir, cwd);

            printf("%s\n", new_dir);

            free(new_dir);
        }
    }

    free_tokens(tokens);

    return TRUE;
}

int exit1()
{
    return FALSE;
}

int getpath(char** tokens, char** history __attribute__((unused)), const int* front __attribute__((unused)), const int* rear __attribute__((unused)))
{
    if(tokens[1] == NULL){
        const char *s = getenv("PATH");
        printf("PATH is currently set to :%s\n", (s != NULL) ? s : "getenv returned NULL");

        free_tokens(tokens);

        return TRUE;
    }
    else{
        fprintf(stderr, "Invalid number of parameters for getpath\n");

        free_tokens(tokens);

        return ERROR;

    }
}

int setpath(char** tokens, char** history __attribute__((unused)), const int* front __attribute__((unused)), const int* rear __attribute__((unused)))
{

    if(tokens[1]==NULL||tokens[2]!=NULL)
    {
        fprintf(stderr, "Invalid number of parameters for setpath\n");

        free_tokens(tokens);

        return ERROR;
    }

    char * isDir;
    char path[100];
    const char delim[2]=":";
    isDir=strtok(tokens[1],delim);
    strcpy(path,isDir);

    while(isDir!=NULL)
    {
        DIR* dir=opendir(isDir);
        if(dir){}
        else if(ENOENT==errno)
        {
            printf("The directory %s is not a valid PATH\n",isDir);

            free_tokens(tokens);

            free(dir);

            return -1;
        }

        isDir=strtok(NULL,delim);
        if(isDir!=NULL)
        {
            strcat(path,delim);
            strcat(path,isDir);

        }

    }

    setenv("PATH",path,1);
    free(isDir);

    free_tokens(tokens);

    return TRUE;

}


