/**
 * Header for declaring methods and constants
 */

#define TRUE 1
#define FALSE 0
#define ERROR (-1)
#define HISTORY_SIZE 20
#define MAX_INPUT_LENGTH 514
#define COMMANDS_LENGTH 5
#define DELIMITER_SIZE 8
#define MAX_TOKENS 50
#define MAX_DEPTH 20

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>

struct alias{
    char* key;
    char* value;
    struct alias *next;
};

typedef struct alias Alias;
typedef struct alias** AList;

AList aliases;


/**
 * Contains methods to print the command prompt, read user input and tokenise it.
 * @authors Eamonn McClay, Mark Oppo, Balazs Szalay
 * @date 28/01/2021
 */

/**
 * Prints the prompt for the user to enter a command.
 * @return      TRUE always
 */
int print_prompt() {
    printf("> ");
    return TRUE;
}

/**
 * Reads the users input as a string, handles long input.
 * @return      string from user input
 */
char* read_input() {

    char* input = malloc(sizeof(char)*MAX_INPUT_LENGTH);
    fgets(input, MAX_INPUT_LENGTH, stdin);

    if(input == NULL || *input == '\0') {		// Ctrl-D detected
        return NULL;
    }

    if(strlen(input) == 1) {	            // Empty input
        fflush(stdin);
        return input;
    }

    if(input[strlen(input)-1] != '\n' && strlen(input) < MAX_INPUT_LENGTH-1) {
        return NULL;
    }

    if(input[strlen(input)-1] != '\n' && strlen(input) >= MAX_INPUT_LENGTH-1) {
        printf("Input too long\n");
        fflush(stdin);
        memset(input, '\0', sizeof(char) * MAX_INPUT_LENGTH);

        input[0] = '\n';
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
        return input;
    }

    return input;

}

/**
 * Removes leading and trailing whitespace from a string
 * @param str   string that whitespaces should be removed from
 * @return      starting char of string
 */
char* trim_whitespace(char* str)
{
    char* end;

    // Trim leading space
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

/**
 * Function to take in an input string and split it into separate tokens.
 * Returns a pointer to a null-terminated list of char pointers.
 * Will return no more than 50 tokens, as well as the null token
 * @param input         user input
 * @return              tokenised string
 */
char** tokenise_string(char* input) {

    if(input == NULL || input[0] == '\0'){
        return NULL;
    }

    char delimiters[DELIMITER_SIZE] = {' ', '\t', '|', '>', '<', '&', ';', '\n'};
    int n_token = 1;
    char* current = trim_whitespace(input);

    while (*current != '\0' && n_token < MAX_TOKENS) {
        for (int i = 0; i < DELIMITER_SIZE; i++) {
            if (*current == delimiters[i]) {
                n_token ++;
                *current = ' ';
                break;
            }
        }
        current ++;
    }

    char** tokens = NULL;
    tokens = (char**) malloc(sizeof(char*) * (n_token+1));
    for(int i = 0; i < n_token; ++i){
        tokens[i] = (char*) malloc(sizeof (char) * MAX_INPUT_LENGTH);
        memset(tokens[i], '\0', MAX_INPUT_LENGTH);
    }

    current = input;
    n_token = 0;
    int prev_is_delim = 1;

    while (*current != '\0' && n_token < MAX_TOKENS) {
        if (*current == ' ' || *current == '\n') {
            prev_is_delim = 1;
            *current = '\0';
            current++;
        }
        else if (prev_is_delim == 1) {
            prev_is_delim = 0;
            if (n_token != MAX_TOKENS) {
                int i;
                for(i = 0; *current != ' ' && strcmp(current, "") != 0; ++i){
                    current++;
                }
                current -= i;
                strncpy(tokens[n_token], current, i);
                n_token ++;
                current += i;
            }
        }
    }

    if(prev_is_delim == 1){
        free(tokens[n_token]);
        tokens[n_token] = NULL;
    }
    else{
        tokens[n_token] = NULL;
    }

    return tokens;
}

/**
 * Frees up the used tokens
 * @param tokens    tokens to be freed up
 */
void free_tokens(char** tokens){
    int n_tokens;
    for(n_tokens = 0; tokens[n_tokens] != NULL; ++n_tokens){}

    for(int j = 0; j < n_tokens; ++j){
        free(tokens[j]);
    }

    free(tokens);
}


/**
 * Contains methods for handling the aliases, including methods to add, print and remove aliases.
 *
 * @authors Balazs Szalay, Eamonn McClay, Regina Gyimesi, Zoltan Kiss
 * @date 26/02/2021
 */

/**
 * Initialises a list for storing aliases
 */
void make_aliases()
{
    aliases = malloc(sizeof(Alias*));
    *aliases = NULL;
}

/**
 * Returns the list of aliases
 */
Alias* get_aliases() {
    return *aliases;
}

/**
 * Method for creating a single line of string from arrays of strings
 * @param tokens    arrays of string
 * @param value     variable where the string should be stored
 * @return          pointer to the string that has been created
 */
char* detokenize(char** tokens, char* value){
    int i;
    for(i = 0; tokens[i] != NULL && strcmp(tokens[i], "") != 0; ++i){
        char* temp = tokens[i];

        if(value[0] == '\0'){
            strcpy(value, temp);
        }
        else{
            strcat(value, " ");
            strcat(value, temp);
        }

    }

    strcat(value, "\0");

    return value;
}

/**
 * Creates a new alias list element
 * @param key       key for the alias
 * @param value     value that the alias will be representing
 * @return          new alias element
 */
Alias *new_alias(char* key, char*value){

    Alias* temp = (Alias*) malloc(sizeof(Alias));
    temp->key = malloc(sizeof (char) * MAX_INPUT_LENGTH);
    temp->value = malloc(sizeof (char) * MAX_INPUT_LENGTH);

    memset(temp->key, '\0', MAX_INPUT_LENGTH);
    memset(temp->value, '\0', MAX_INPUT_LENGTH);

    strcpy(temp->key, key);
    strcpy(temp->value, value);
    temp->next = NULL;
    return temp;
}

/**
 * Adds a new alias to the aliases list
 * @param key       key for the alias
 * @param value     value that the alias will be representing
 */
void add_alias(char* key, char* value)
{

    Alias* newN = new_alias(key,value);

    Alias* current = *aliases;
    if(current == NULL)
    {
        *aliases = newN;
    }
    else {
        newN->next = current;
        *aliases = newN;
    }

}

/**
 * Prints out the currently set aliases
 */
void print_alias()
{

    Alias* curr = *aliases;
    if(curr == NULL || *aliases == NULL)
    {
        printf("No aliases have been set\n");
        return;
    }

    while(curr!=NULL)
    {
        printf("%s %s\n",curr->key,curr->value);
        curr=curr->next;
    }

}

/**
 * If a new alias key is already present in the list, this method replaces
 * the value of the old alias with the new value
 * @param key       aliases key
 * @param tokens    value that it will be replaced with
 * @return          TRUE if alias with such key was present
 *                  ERROR otherwise
 */
int replace_if_exists(char* key, char** tokens)
{

    Alias* current = *aliases;
    while(current!=NULL)
    {
        if(strcmp(current->key,key)==0)
        {
            memset(current->value, '\0', MAX_INPUT_LENGTH);

            char value[MAX_INPUT_LENGTH] = {'\0'};

            detokenize(tokens, value);

            strcpy(current->value, value);

            printf("Alias was replaced\n");

            return TRUE;

        }
        else
        {
            current=current->next;
        }

    }
    return ERROR;

}

/**
 * This method handles adding and replacing aliases.
 * @param key           key for alias
 * @param tokens        value of the new alias
 * @return              TRUE if alias with such key was present
 *                      ERROR it is a brand new alias
 */
int add_replace(char* key, char** tokens)
{
    int status = replace_if_exists(key, tokens);
    if(status == ERROR)
    {
        char value[MAX_INPUT_LENGTH];
        memset(value, '\0', MAX_INPUT_LENGTH);
        detokenize(tokens, (char *) value);
        add_alias(key, (char *) value);
        return TRUE;
    }
    return status;

}

/**
 * Deletes a alias with the specified key
 * @param key       the alias key that will be deleted
 * @return          TRUE, if alias was deleted successfully
 *                  ERROR, if no such alias is present or no aliases have been set
 */
int delete_alias(char* key)
{

    Alias* curr = *aliases;
    Alias* previous = NULL;
    if(curr == NULL)
    {
        printf("No aliases have been set\n"); //no aliases have been set invalid operation
        return ERROR;
    }
    else if(strcmp(curr->key,key)==0) //if first element is what we're looking for
    {
        *aliases = curr->next;

        free(curr->key);
        free(curr->value);
        curr->value = NULL;
        free(curr);
        return TRUE;
    }
    while(curr->next!=NULL) //iterate to the end of the list
    {
        previous=curr;
        curr=curr->next;

        if(strcmp(curr->key,key)==0)
        {
            if(curr->next!=NULL)
            {
                previous->next=curr->next;

                free(curr->key);
                free(curr->value);
                free(curr);
                return TRUE;
            }
            else{
                previous->next=NULL;

                free(curr->key);
                free(curr->value);
                free(curr);
                return TRUE;
            }

        }

    }

    printf("Alias with these parameters was not found\n");
    return ERROR;
}

/**
 * This method handles the possible alias commands
 * @param tokens        user input string tokenized
 * @return              tokens of the alias value or NULL if something went wrong
 */
char** check_alias(char** tokens) {

    if(tokens == NULL || *tokens == NULL){      // If somehow the aliases list does not exist or no aliases present
        fprintf(stderr, "Invalid tokens or no aliases\n");
        free_tokens(tokens);
        return NULL;
    }

    if(tokens[1] != NULL && tokens[2] != NULL && strcmp(tokens[2],"")==0 && strcmp(tokens[2],"alias")==0){  // If something bad happens during tokenizing
        fprintf(stderr, "Invalid tokens or no aliases\n");
        free_tokens(tokens);
        return NULL;
    }

    // If alias does not have the correct number of parameters
    if(strcmp(tokens[0],"alias")==0 && tokens[1]!=NULL && strcmp(tokens[1], "") != 0 && (tokens[2]==NULL || strcmp(tokens[2], "") == 0)){
        fprintf(stderr, "Alias needs 2 parameters, only 1 was given\n");
        free_tokens(tokens);
        return NULL;
    }

    // If the user wants add or replace a new alias
    if(strcmp(tokens[0],"alias")==0 && tokens[1]!=NULL && tokens[2]!=NULL)
    {
        char key_temp[MAX_INPUT_LENGTH];
        memset(key_temp, '\0', MAX_INPUT_LENGTH);
        strcpy(key_temp, tokens[1]);

        add_replace(key_temp, &tokens[2]);

        free_tokens(tokens);
        return NULL;

    }
        // If the user wants to print out the aliases
    else if(strcmp(tokens[0],"alias")==0 && (tokens[1]==NULL || (strcmp(tokens[1], "") == 0)))
    {
        print_alias();
        free_tokens(tokens);
        return NULL;
    }
        // If the user wants to unalias a alias
    else if (strcmp(tokens[0],"unalias")==0 && tokens[1]!=NULL && tokens[2]==NULL)
    {
        delete_alias(tokens[1]);
        free_tokens(tokens);
        return NULL;
    }
        // None of the above
    else{
        free_tokens(tokens);
        return NULL;
    }

}


/**
 * Creates or opens .aliases file, saves every line of alias with the corresponding key and closes
 * when there is no more to write.
 * @return  TRUE, if save was successful
 */
int save_aliases() {
    Alias* current = *aliases;
    char* filepath = malloc(sizeof(char) * MAX_INPUT_LENGTH);
    memset(filepath, '\0', MAX_INPUT_LENGTH);

    strcpy(filepath, getenv("HOME"));
    strcat(filepath, "/.aliases");
    FILE* file = fopen(filepath, "w+");

    while(current!=NULL){
        char temp_key[MAX_INPUT_LENGTH];
        char temp_value[MAX_INPUT_LENGTH];

        memset(temp_key, '\0', MAX_INPUT_LENGTH);
        memset(temp_value, '\0', MAX_INPUT_LENGTH);

        strcpy(temp_key, current->key);
        strcpy(temp_value, current->value);

        fprintf(file, "%s %s\n", temp_key, temp_value); // writes key and the corresponding alias to file
        current = current->next;
    }

    fclose(file);
    free(filepath);
    return TRUE;
}

/**
 * This method deletes aliases and frees up the allocated memory
 * @return      TRUE if everything went alright
 *              ERROR if aliases list does not exist
 */
int free_aliases(){
    Alias* current = *aliases;
    Alias* prev = NULL;

    if(current==NULL)
    {
        return ERROR;
    }

    while(current!=NULL){
        prev = current;
        current = current->next;

        free(prev->key);
        prev->key = NULL;
        free(prev->value);
        prev->value = NULL;
        free(prev);
    }

    *aliases = NULL;

    return TRUE;
}

/**
 * Opens .aliases file, reads in every line and adds to alias list.
 * @return  TRUE, if load was successful
 *          ERROR, if alias save file was corrupted or does not exist
 */
int load_aliases() {
    char* filepath = malloc(sizeof(char) * MAX_INPUT_LENGTH);
    memset(filepath, '\0', MAX_INPUT_LENGTH);
    strcpy(filepath, getenv("HOME"));
    strcat(filepath, "/.aliases");
    FILE* file = fopen(filepath, "r+");

    char line[MAX_INPUT_LENGTH];
    char temp[MAX_INPUT_LENGTH];
    char* key;
    char* value;

    memset(line, '\0', MAX_INPUT_LENGTH);
    memset(temp, '\0', MAX_INPUT_LENGTH);

    if(!file) {
        fprintf(stderr, "Aliases file could not be located\n");

        free(filepath);
        return ERROR;
    } // Returns error if .aliases file not located
    else {
        while(fgets(line, sizeof(line), file)) {

            int i;
            for(i = 0; i < strcspn(line, "\n"); ++i){
                temp[i] = line[i];
            }
            temp[i] = '\0';

            key = strtok(temp, " ");
            if(key == NULL){
                fprintf(stderr, "Alias file is corrupt, file read stopped and aliases deleted.\n");
                free_aliases();
                aliases = NULL;
                make_aliases();
                free(filepath);
                fclose(file);
                return ERROR;
            }
            value = temp + strlen(key) + 1;
            strcat(value, "\0");

            add_alias(key, value);
        }
    } // Other wise adds alias grabbed from each line

    fclose(file);
    free(filepath);
    return TRUE;
}

/**
 * Get the alias value for a key
 * @param key   alias key to be searched for
 * @return      value of alias
 */
char* getvalue(char* key)
{
    Alias* current = *aliases;
    if (current==NULL)
    {
        return NULL;
    }
    else
    {
        while (current!=NULL)
        {
            if (strcmp(key,current->key)==0)
            {
                return current->value;
            }
            current=current->next;
        }
        return NULL;
    }


}

/**
 * This method replaces a input string with the aliased version, if such string part found
 * @param tokens    user input string
 * @return          new substituted string
 */
char** substituteAlias(char** tokens)
{
    char temp[MAX_INPUT_LENGTH];
    int index = 0;
    while (tokens[index] != NULL)
    {
        if (strcmp(tokens[0],"unalias")==0||strcmp(tokens[0],"alias")==0)
        {
            return tokens;
        }
        if (getvalue(tokens[index])!= NULL) {
            char copy[MAX_INPUT_LENGTH];

            strcpy(copy, getvalue(tokens[index]));

            if(index == 0) {
                strcpy(temp, copy);
            }
            else{
                strcat(temp, " ");
                strcat(temp, copy);
            }

        }
        else{
            char copy[MAX_INPUT_LENGTH];

            strcpy(copy, tokens[index]);

            if(index == 0) {
                strcpy(temp, copy);
            }
            else{
                strcat(temp, " ");
                strcat(temp, copy);
            }

        }

        index++;
    }

    char final[MAX_INPUT_LENGTH];
    memset(final, '\0', MAX_INPUT_LENGTH);
    strcpy(final, temp);
    memset(temp, '\0', MAX_INPUT_LENGTH);

    int n_tokens;
    for(n_tokens = 0; tokens[n_tokens] != NULL; ++n_tokens){}

    for(int j = 0; j < n_tokens; ++j){
        free(tokens[j]);
    }

    free(tokens);

    return tokenise_string(final);
}


/**
 * Code for our inbuilt, non-linux commands: cd, exit, getpath, setpath
 * @authors Balazs Szalay, Regina Gyimesi
 * @date 11/02/2021
 */

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



/**
 * Contains methods for handling the command history, including methods to store, print and execute previous commands.
 *
 * @authors Mark Oppo, Regina Gyimesi, Eamonn McClay, Zoltan Kiss
 * @date 10/02/2021
 */

char *history[HISTORY_SIZE];
int front = -1;
int rear = -1;

/**
 * This method returns the number of elements in the history
 * @return      number of elements in history
 */
int number_of_elements_in_history() {

    int i;
    for (i = 0; i < HISTORY_SIZE && history[i] != NULL && strcmp(history[i], "") != 0; ++i) {}

    return i;
}

/**
 * Initialises the history array
 */
void make_history() {
    for (int k = 0; k < HISTORY_SIZE; k++) {
        history[k] = (char *) malloc(sizeof(char) * MAX_INPUT_LENGTH);
        memset(history[k], '\0', MAX_INPUT_LENGTH);
    }
}

/**
 * Frees up the history and deletes it
 */
void free_history() {
    for (int k = 0; k < HISTORY_SIZE; k++) {
        free(history[k]);
    }
}

/**
 * This method checks if the history array is empty, does not contain any elements
 * @return      TRUE if it is empty FALSE otherwise
 */
int is_history_empty() {
    for (int i = 0; i < HISTORY_SIZE; ++i) {
        if (*history[i] != '\0') {
            return FALSE;
        }
    }

    return TRUE;
}

/**
 * This method checks if the array is full
 * @return TRUE if it is full, FALSE otherwise
 */
int is_history_full() {
    // The array is full if:
    //      - the rear is the last available position
    //      - the rear and the front%HISTORY_SIZE are the same, the two flags are at the same position in the array
    if ((front == 0 && rear == HISTORY_SIZE - 1) || (rear == (front - 1) % (HISTORY_SIZE - 1))) {
        return TRUE;
    }

    return FALSE;
}

/**
 * This method adds a history element to the history array
 * @param input     string that will be added
 * @return          TRUE if adding was successful, ERROR otherwise
 */
int add_to_history(char* input) {

    if (input == NULL || strcmp(input, "\n") == 0 || strcmp(input, "\0") == 0) {
        fprintf(stderr, "Input error, not adding to history\n");
        return ERROR;
    }

    if (is_history_full()) {            // in a queue we wouldn't overwrite values, but here it can be done
        front += 1;
        if (front == HISTORY_SIZE) {    // the front reached the end of the array, so we reset it to the first
            front = 0;
        }
    }

    if (front == -1) {                                      // First element
        front = 0;
        rear = 0;
        memset(history[rear], '\0', MAX_INPUT_LENGTH);
        strcpy(history[rear], input);
    }
    else if (rear == HISTORY_SIZE - 1 && front != 0) {    // Rear reached the end, reset to the first element
        rear = 0;
        memset(history[rear], '\0', MAX_INPUT_LENGTH);
        strcpy(history[rear], input);
    }
    else {                                                // Rear is somewhere between the first and last position
        rear += 1;
        memset(history[rear], '\0', MAX_INPUT_LENGTH);
        strcpy(history[rear], input);
    }

    if (history[rear] == NULL) {
        fprintf(stderr, "Nothing has been added to the history!");
        return ERROR;
    }

    return TRUE;
}


/**
 * Executes a ![1-20] history invocation
 * @param number    number of the history element to be executed (user format)
 * @return          string that the history contains at that number
 */
char** exec_number_history(int number) {

    int temp = 0;
    int i;

    number--;                                  // Decreasing to make it equal to array index representation

    if (number >= 0 && number <= (HISTORY_SIZE - 1)) {        // Checking if number is between the available number of commands

        for (i = front; temp < number; ++i) {  // From here we need to find the index we are looking for
            if (i == HISTORY_SIZE) {           // Because 'i' is different then the actual order, we have to match it
                i = 0;
            }
            temp++;
        }

        if (i == HISTORY_SIZE) {               // This is a edge case, because the loop, does not set i to
            i = 0;                             // 0 if, temp becomes equal to the number and we reached the end of the array
        }

        if (i == -1) {                         // This is a edge case, because the loop, does not set i to
            i = 0;                             // 0 if, temp becomes equal to the number and we reached the end of the array
        }

        if (*history[i] != '\0') {             // Copying and tokenizing the history command
            char line[MAX_INPUT_LENGTH] = {'\0'};
            memset(line, '\0', MAX_INPUT_LENGTH);
            strcpy(line, history[i]);
            return tokenise_string(line);
        }
    }

    fprintf(stderr, "History cannot be executed\n");
    if (number >= number_of_elements_in_history()) {
        fprintf(stderr, "Number is greater, than the number of history elements.\n");
    }
    else if (number == -1) {
        fprintf(stderr, "Invalid invocation of history. Use case: ![!][-][1-20]\n");
    }

    return NULL;
}

/**
 * This is a special function, that checks, if there is a alias for history command !-1
 * If so, it returns the key for that alias
 * @return      key for alias value !-1, if no such alias, it returns NULL
 */
char* check_for_minus1() {

    Alias *curr = get_aliases();

    if (curr == NULL) {
        return NULL;
    }
    else if (strcmp(curr->value, "!-1") == 0) {      // If first element is what we're looking for
        return curr->key;
    }

    while (curr->next != NULL) {                     // Iterate to the end of the list
        curr = curr->next;

        if (strcmp(curr->value, "!-1") == 0) {       // If first element is what we're looking for
            return curr->key;
        }
    }

    return NULL;
}

/**
 * Executes a !-[1-20] history invocation
 * @param number  number of the history element to be executed (user format)
 * @return        string that the history contains at that number
 */
char** exec_minus_number_history(int number) {

    int temp = 0;
    int i;

    char *key_for_minus1 = check_for_minus1();
    char *key_copy_for_minus1 = malloc(sizeof(char) * MAX_INPUT_LENGTH);
    if (key_for_minus1 != NULL) {
        strcpy(key_copy_for_minus1, key_for_minus1);
        strcat(key_copy_for_minus1, "\n");
    }

    number--;                                   // Decreasing to make it equal to array index representation

    if (number >= 0 &&
        number <= (HISTORY_SIZE - 1)) {         // Checking if number is between the available number of commands

        for (i = rear; temp < number; --i) {    // From here we need to find the index we are looking for
            if (i == -1) {                      // Because 'i' is different then the actual order, we have to match it
                i = HISTORY_SIZE - 1;
            }
            temp++;
        }

        if (i == -1) {                          // This is a edge case, because the loop, does not set i to
            i = HISTORY_SIZE - 1;               // 0 if, temp becomes equal to the number and we reached the end of the array
        }

        if (*history[i] != '\0') {              // Copying and tokenizing the history command
            char line[MAX_INPUT_LENGTH] = {'\0'};

            if (key_copy_for_minus1 != NULL && strcmp(history[i], key_copy_for_minus1) == 0) {  // If there is a key for alias !-1, we execute the previous history
                strcpy(line, history[i - 1]);
            }
            else {
                strcpy(line, history[i]);
            }

            free(key_copy_for_minus1);

            return tokenise_string(line);
        }
    }

    fprintf(stderr, "History cannot be executed\n");
    if (number >= number_of_elements_in_history()) {
        fprintf(stderr, "Number is greater, than the number of history elements.\n");
    }
    else if (number == -1) {
        fprintf(stderr, "Invalid invocation of history. Use case: ![!][-][1-20]\n");
    }

    free(key_copy_for_minus1);

    return NULL;
}

/**
 * Converts to number from the !<no.> text.
 * @param text      string that contains number
 * @return          number in integer format
 */
int convert_number_to_int(const char* text) {

    int number = 0;
    int offset = '0';                       // offset between ASCII value of number and actual number

    for (int i = 1; text[i] != '\0'; i++) {
        if (text[i] == '-' && i == 1) continue;
        if (text[i] >= '0' && text[i] <= '9') {
            number = (number * 10) + (text[i] - offset);
            if (number < 0) return ERROR;   // overflow protection
        } else {
            fprintf(stderr, "Invalid invocation of history. Use case: ![!][-][1-20]\n");
            return ERROR;
        }
    }

    return number;
}

/**
 * Executes a !! history invocation
 * @return  string that the history contains at that number
 */
char** exec_recent_history() {
    if (*(history[0]) != '\0') {
        return exec_minus_number_history(1);
    }

    fprintf(stderr, "No commands stored in recent history\n");
    return NULL;
}

/**
 * Prints out the contents of the history to the terminal, in a user friendly form
 * @param tokens        user input
 * @return              TRUE, if tje print was successful, ERROR otherwise
 */
int print_history(char** tokens) {

    int number_printed = 0;         // for bug fixing

    if (*history == NULL || tokens[1] != NULL) {
        fprintf(stderr, "Invalid invocation of history. Use case: ![!][-][1-20]\n");

        free_tokens(tokens);

        return ERROR;
    }

    if (!is_history_empty()) {
        int i;
        int index = 1;      // The user friendly numbering
        for (i = front; i != rear; ++i) {
            if (i == HISTORY_SIZE) {
                i = 0;
            }

            if (number_printed == HISTORY_SIZE) {
                free_tokens(tokens);
                return TRUE;
            }

            printf("%d. %s\n", index, history[i]);
            ++index;

            number_printed++;
        }

        if (number_printed < HISTORY_SIZE) {
            printf("%d. %s\n", index, history[i]);
            ++index;
        }

        free_tokens(tokens);
        return TRUE;
    }

    fprintf(stderr, "No commands stored in history\n");

    free_tokens(tokens);
    return ERROR;
}

/**
 * This method checks the type of the history invocation from the user input
 * @param tokens    user input
 * @return          tokenised string from history
 */
char** check_history_type(char** tokens) {
    char first_token[MAX_INPUT_LENGTH];
    strcpy(first_token, tokens[0]);                         // Storing the first token as single string
    first_token[MAX_INPUT_LENGTH - 1] = '\0';

    if (first_token[0] == '!') {                                // Checking string chars one-by-one
        if (first_token[1] == '!' && first_token[2] == '\0' && tokens[1] == NULL) {

            free_tokens(tokens);

            return exec_recent_history();
        }
        else if (first_token[1] == '-' && tokens[1] == NULL) {

            if (strlen(tokens[0]) > 4) {
                memset(tokens[0], '\0', MAX_INPUT_LENGTH);
                fprintf(stderr, "Invalid invocation of history. Use case: ![!][-][1-20]\n");
                free_tokens(tokens);
                return NULL;
            }

            int number = convert_number_to_int(tokens[0]);

            free_tokens(tokens);

            return exec_minus_number_history(number);

        }
        else if (first_token[1] >= '0' && first_token[1] <= '9' && tokens[1] == NULL) {
            if (strlen(tokens[0]) > 4) {
                memset(tokens[0], '\0', MAX_INPUT_LENGTH);
                fprintf(stderr, "Invalid invocation of history. Use case: ![!][-][1-20]\n");
                free_tokens(tokens);
                return NULL;
            }

            int number = convert_number_to_int(tokens[0]);

            free_tokens(tokens);

            return exec_number_history(number);
        }
        else if (strcmp(first_token, "!clear") == 0 && tokens[1] == NULL) {
            free_history();
            make_history();
            front = -1;
            rear = -1;
            printf("History has been reset!\n");

            free_tokens(tokens);

            return NULL;
        }
        else {
            fprintf(stderr, "Invalid invocation of history. Use case: ![!][-][1-20]\n");

            free_tokens(tokens);

            return NULL;
        }

    }

    return tokens;

}

/**
 * Creates or opens .hist_list file, saves every line of history to it and closes
 * when there is no more to write.
 * @return  TRUE if the saving was successful
 */
int save_history() {

    int number_printed = 0;
    int i;

    char *filepath = malloc(sizeof(char) * MAX_INPUT_LENGTH);
    strcpy(filepath, getenv("HOME"));
    strcat(filepath, "/.hist_list");
    FILE* file = fopen(filepath, "w+");

    for (i = front; i != rear; ++i) {
        if (i == HISTORY_SIZE) {
            i = 0;
        }

        if (number_printed == HISTORY_SIZE) {
            fclose(file);
            free(filepath);
            return TRUE;
        }

        if (strcmp(history[i], "") != 0 && strcmp(history[i], "\n") != 0 && strcmp(history[i], "\0") != 0) {
            fprintf(file, "%s\n", history[i]);
        }
        else {
            number_printed++;
            continue;
        }

        number_printed++;
    }

    fprintf(file, "%s\n", history[i]);
    fclose(file);
    free(filepath);
    return TRUE;
}

/**
 * Loads history from .hist_list file in home directory
 * @return  TRUE if load was successful
 */
int load_history() {

    char* filepath = malloc(sizeof(char) * MAX_INPUT_LENGTH);
    strcpy(filepath, getenv("HOME"));
    strcat(filepath, "/.hist_list");
    FILE* file = fopen(filepath, "r+");
    char line[MAX_INPUT_LENGTH];

    if (!file) {
        fprintf(stderr, "History file could not be located\n");
        free(filepath);
        return ERROR;
    }
    else {
        while (fgets(line, sizeof(line), file)) {
            strtok(line, "\n");
            add_to_history(line);
        }
    }

    fclose(file);
    free(filepath);
    return TRUE;
}


/**
 * Taking an array of tokenized inputs and executing the proper command.
 *
 * @authors Balazs Szalay, Zoltan Kiss
 * @date 01/02/2021
 */

int apply_command(char** tokens) {

    char *builtin_str[] = {
            "exit",
            "getpath",
            "setpath",
            "cd",
            "history"
    };

    int (*builtin_func[])(char **) = {
            &exit1,
            &getpath,
            &setpath,
            &change_directory,
            &print_history
    };

    if (tokens == NULL) {       // If the first input is null or a NULL char, then we return 0 thus indicating it is an exit
        return FALSE;
    }

    if (*tokens == NULL) {      // This happens if the user only pressed Enter, and nothing else as input
        free(tokens);
        return ERROR;
    }

    if (*tokens[0] == '\0') {   // If the first input is null or a NULL char, then we return 0 thus indicating it is an exit
        free(tokens);
        return FALSE;
    } else {

        char* history_line = malloc(sizeof(char) * MAX_INPUT_LENGTH);
        memset(history_line, '\0', MAX_INPUT_LENGTH);
        detokenize(tokens, history_line);

        for (int i = 0; i < MAX_DEPTH && tokens != NULL; ++i) {
            tokens = substituteAlias(tokens);
        }

        if (tokens[0] != NULL && *(tokens[0]) != '\0' && *(tokens[0]) != '\n' && *(tokens[0]) != '!') {
            add_to_history(history_line);
        }

        free(history_line);

        if (tokens != NULL && (*tokens)[0] == '!') {
            tokens = check_history_type(tokens);
        }

        for (int i = 0; i < MAX_DEPTH && tokens != NULL; ++i) {
            tokens = substituteAlias(tokens);
        }

        if (tokens != NULL && (*tokens)[0] == '!') {
            tokens = check_history_type(tokens);
        }

        if (tokens != NULL && (strcmp(tokens[0], "alias") == 0 || strcmp(tokens[0], "unalias") == 0)) {
            tokens = check_alias(tokens);
        }

        if (tokens != NULL) {

            for (int i = 0; i < COMMANDS_LENGTH; ++i) {
                if (strcmp(tokens[0], builtin_str[i]) == 0)     // Checking if the input is an inbuilt function and if so calling it
                    return (*builtin_func[i])(tokens);          // with the arguments provided with it
            }

            // Else creating a Unix call and passing in the tokenized the arguments

            pid_t pid;
            pid = fork();

            if (pid < 0) {

                printf("Fork failed");
                free_tokens(tokens);
                return TRUE;
            } else if (pid == 0) {

                char temp[MAX_INPUT_LENGTH] = {'\0'};
                strcpy(temp, tokens[0]);

                execvp(temp, tokens);
                fprintf(stderr, "%s", tokens[0]);
                perror(" is not a valid command");

                free_tokens(tokens);
                exit(EXIT_FAILURE);
            } else {
                wait(NULL);
                free_tokens(tokens);
                return TRUE;
            }
        }

    }

    return TRUE;

}

/**
 * CS210 Simple Shell - Group 5
 * Balazs Szalay, Eamonn McClay, Mark Oppo, Regina Gyimesi, Zoltan Kiss
 *
 * The main driver file
 */

int main()
{
    char *line;
    char **args;
    int status;
    char *start_path = getenv("PATH");      // Initial path stored

    set_directory();
    make_history();
    make_aliases();
    load_history();
    load_aliases();

    do {

        print_prompt();
        line = read_input();

        args = tokenise_string(line);

        status = apply_command(args);

        free(line);
    } while (status);

    save_aliases();
    save_history();
    free_history();
    free_aliases();
    setenv("PATH", start_path, 1); // initial path restored
    printf("Path is restored to %s\n",getenv("PATH"));

    return 0;
}