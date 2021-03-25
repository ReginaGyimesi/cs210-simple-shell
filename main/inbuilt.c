/**
 * Code for our inbuilt, non-linux commands: cd, exit, getpath, setpath
 * @authors Balazs Szalay, Regina Gyimesi
 * @date 11/02/2021
 */
#include "main.h"

/**
 * Gets the HOME directory and sets it. Checks if it has been correctly set,
 * gets and sets directory to HOME, if home directory is null then exists
 * @return  TRUE if HOME directory change was successful, FALSE otherwise
 */
int set_directory() {
    if (chdir(getenv("HOME")) != 0) {
        perror("Failed");
    }

    // Checks if current directory is set to home
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    }
    else {
        perror("getcwd() error");
        return TRUE;
    }
    return FALSE;
}

/**
 * Function to change directory
 * @param tokens    user input
 * @return
 */
int change_directory(char** tokens) {

    // Initial error checks
    if (tokens == NULL || *tokens == NULL || tokens[0] == NULL) {
        perror("NULL pointer error in cd command\n");
        return ERROR;
    }

    if (tokens[1] == NULL) {                    // No parameter were given, set to user's home directory
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
    else {                                  // Some parameter were given, execute cd with that param
        if (chdir(tokens[1]) != 0) {  // Disregarding other params, only using the next param after cd
            char *s = malloc(sizeof(char *) * 100);
            strcpy(s, "Failed to change to ");
            strcat(s, tokens[1]);
            strcat(s, ". Use case: cd [path/to/directory]");
            perror(s);
            free(s);

            free_tokens(tokens);

            return ERROR;
        }
        else {                              // Checks if current directory is set to home
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

/**
 * This method exits the shell
 * @param tokens    user input (used for free-ing)
 * @return          always returns FALSE
 */
int exit1(char** tokens)
{
    free_tokens(tokens);
    return FALSE;
}

/**
 * Prints out to the terminal the current PATH that is set.
 * @param tokens        user input
 * @return              TRUE is printing was successful, ERROR otherwise
 */
int getpath(char** tokens)
{
    if(tokens[1] == NULL){
        const char *s = getenv("PATH");
        printf("PATH is currently set to :%s\n", (s != NULL) ? s : "getenv returned NULL");

        free_tokens(tokens);

        return TRUE;
    }
    else{
        fprintf(stderr, "Invalid number of parameters for getpath, it takes no parameters!\n");

        free_tokens(tokens);

        return ERROR;

    }
}

/**
 * This method sets the PATH to a user defined one.
 * @param tokens        user input
 * @return              TRUE if setting was successful, ERROR otherwise
 */
int setpath(char** tokens)
{

    if(tokens[1] == NULL || tokens[2] != NULL)
    {
        fprintf(stderr, "Invalid number of parameters for setpath, it takes exactly 1 parameter!\n");

        free_tokens(tokens);

        return ERROR;
    }

    char* isDir;
    char path[MAX_INPUT_LENGTH] = {'\0'};
    const char delim[2] = ":";

    isDir = strtok(tokens[1],delim);
    strcpy(path,isDir);
    path[MAX_INPUT_LENGTH-1] = '\0';

    while(isDir != NULL)
    {
        DIR* dir = opendir(isDir);
        if(dir){}
        else if(ENOENT == errno)
        {
            printf("The directory %s is not a valid PATH\n",isDir);

            free_tokens(tokens);

            free(dir);

            return ERROR;
        }

        isDir=strtok(NULL,delim);

        if(isDir != NULL)
        {
            strcat(path,delim);
            strcat(path,isDir);

        }

    }

    path[MAX_INPUT_LENGTH-1] = '\0';

    setenv("PATH",path,1);
    free(isDir);

    free_tokens(tokens);

    return TRUE;

}