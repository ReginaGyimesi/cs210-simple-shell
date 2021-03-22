//
// Created by Zoltan Kiss on 27/01/2021.
//
#define TRUE 1
#define FALSE 0
#define ERROR (-1)
#define HISTORY_SIZE 20
#define MAX_INPUT_LENGTH 514
#define COMMANDS_LENGTH 5
#define DELIMITER_SIZE 8
#define MAX_TOKENS 50

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

//
// Function to take in an input string and split it into separate tokens.
// Returns a pointer to a null-terminated list of char pointers.
// Will return no more than 50 tokens, as well as the null token
// Created by Mark Oppo on 29/01/2021.
//

char *trimwhitespace(char *str)
{
    char *end;

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

char** tokeniseString(char* input) {

    if(input == NULL || input[0] == '\0'){
        return NULL;
    }

    char delimiters[DELIMITER_SIZE] = {' ', '\t', '|', '>', '<', '&', ';', '\n'};
    int n_token = 1;
    char* current = trimwhitespace(input);

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

    while (*current != '\0' && n_token <= MAX_TOKENS) {
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

//
// Contains methods for handling the aliases,
// including methods to add, print and remove aliases.
// Created by Balazs Szalay, Eamonn McClay and Regina Gyimesi on 2021. 02. 26.
//
AList new_list()
{
    AList temp=malloc(sizeof(Alias*));
    *temp=NULL;
    return temp;
}

void free_tokens(char** tokens){
    int n_tokens;
    for(n_tokens = 0; tokens[n_tokens] != NULL; ++n_tokens){}

    for(int j = 0; j < n_tokens; ++j){
        free(tokens[j]);
    }

    free(tokens);
}

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

Alias *new_alias(char* key,char*value){
    Alias *temp = (Alias*) malloc(sizeof(Alias));
    temp->key = malloc(sizeof (char) * MAX_INPUT_LENGTH);
    temp->value = malloc(sizeof (char) * MAX_INPUT_LENGTH);

    memset(temp->key, '\0', MAX_INPUT_LENGTH);
    memset(temp->value, '\0', MAX_INPUT_LENGTH);

    strcpy(temp->key, key);
    strcpy(temp->value, value);
    temp->next = NULL;
    return temp;
}

void add_alias(AList l,char*key,char*value)
{

    Alias* newN = new_alias(key,value);

    Alias* current = *l;
    if(current==NULL)
    {
        *l=newN;
    }
    else{
        newN->next=current;
        *l=newN;
    }

}

void print_alias(AList l)
{

    Alias *curr=*l;
    if(curr==NULL||*l==NULL)
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

int replace_if_exists(AList l, char* key, char** tokens)
{

    Alias *current=*l;
    while(current!=NULL)
    {
        if(strcmp(current->key,key)==0)
        {
            memset(current->value, '\0', MAX_INPUT_LENGTH);

            char value[MAX_INPUT_LENGTH] = {'\0'};

            detokenize(tokens, value);

            strcpy(current->value, value);

            printf("Alias was replaced\n");

            return 1;

        }
        else
        {
            current=current->next;
        }

    }
    return -1;

}

int add_replace(AList l,char*key, char** tokens)
{
    int status = replace_if_exists(l, key, tokens);
    if(status==-1)
    {
        char value[MAX_INPUT_LENGTH];
        memset(value, '\0', MAX_INPUT_LENGTH);
        detokenize(tokens, (char *) value);
        add_alias(l, key, (char *) value);
        return 1;
    }
    return status;

}

int delete_alias(AList l, char*key)
{

    Alias* curr=*l;
    Alias* previous=NULL;
    if(curr==NULL)
    {
        printf("No aliases have been set\n"); //no aliases have been set invalid operation
        return -1;
    }
    else if(strcmp(curr->key,key)==0) //if first element is what we're looking for
    {
        *l = curr->next;

        free(curr->key);
        free(curr->value);
        curr->value = NULL;
        free(curr);
        return 1;
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
                return 1;
            }
            else{
                previous->next=NULL;

                free(curr->key);
                free(curr->value);
                free(curr);
                return 1;
            }

        }

    }

    printf("Alias with these parameters was not found\n");
    return -1;
}

char** check_alias(char** tokens, AList aliases) {

    if(tokens == NULL || *tokens == NULL){
        fprintf(stderr, "Invalid tokens or no aliases\n");
        free_tokens(tokens);
        return NULL;
    }

    if(tokens[1] != NULL && tokens[2] != NULL && strcmp(tokens[2],"")==0 && strcmp(tokens[2],"alias")==0){
        fprintf(stderr, "Invalid tokens or no aliases\n");
        free_tokens(tokens);
        return NULL;
    }

    if(strcmp(tokens[0],"alias")==0 && tokens[1]!=NULL && strcmp(tokens[1], "") != 0 && (tokens[2]==NULL || strcmp(tokens[2], "") == 0)){
        fprintf(stderr, "Alias needs 2 parameters, only 1 was given\n");
        free_tokens(tokens);
        return NULL;
    }

    if(strcmp(tokens[0],"alias")==0 && tokens[1]!=NULL && tokens[2]!=NULL)
    {
        char key_temp[MAX_INPUT_LENGTH];
        memset(key_temp, '\0', MAX_INPUT_LENGTH);
        strcpy(key_temp, tokens[1]);

        add_replace(aliases, key_temp, &tokens[2]);

        free_tokens(tokens);
        return NULL;

    }
    else if(strcmp(tokens[0],"alias")==0 && (tokens[1]==NULL || (strcmp(tokens[1], "") == 0)))
    {
        print_alias(aliases);
        free_tokens(tokens);
        return NULL;
    }
    else if (strcmp(tokens[0],"unalias")==0 && tokens[1]!=NULL && tokens[2]==NULL)
    {
        delete_alias(aliases,tokens[1]);
        free_tokens(tokens);
        return NULL;
    }
    else{
        free_tokens(tokens);
        return NULL;
    }

}


/*
 * Creates or opens .aliases file, saves every line of alias with the corresponding key and closes
 * when there is no more to write.
 */
int save_aliases(AList aliases) {
    Alias* current = *aliases;
    char *filepath = malloc(sizeof(char) * MAX_INPUT_LENGTH);
    memset(filepath, '\0', MAX_INPUT_LENGTH);

    strcpy(filepath, getenv("HOME"));
    strcat(filepath, "/.aliases");
    FILE *file = fopen(filepath, "w+");

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
    return 1;
}


/*
 * Opens .aliases file, reads in every line and adds to alias list.
 */
int load_aliases(AList aliases) {
    char *filepath = malloc(sizeof(char) * MAX_INPUT_LENGTH);
    memset(filepath, '\0', MAX_INPUT_LENGTH);
    strcpy(filepath, getenv("HOME"));
    strcat(filepath, "/.aliases");
    FILE *file = fopen(filepath, "r+");

    char line[MAX_INPUT_LENGTH];
    char temp[MAX_INPUT_LENGTH];
    char *key;
    char *value;

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
            value = temp + strlen(key) + 1;
            strcat(value, "\0");

            add_alias(aliases, key, value);
        }
    } // Other wise adds alias grabbed from each line

    fclose(file);
    free(filepath);
    return TRUE;
}

int free_aliases(AList aliases){
    Alias* current = *aliases;
    Alias* prev = NULL;

    if(current==NULL)
    {
        return -1;
    }

    while(current!=NULL){
        prev = current;
        current = current->next;

        free(prev->key);
        free(prev->value);
        free(prev);

    }

    return 1;
}


char* getvalue(char*key,AList aliases)
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
char** substituteAlias(char** tokens,AList aliases)
{
    char temp[MAX_INPUT_LENGTH];
    int index=0;
    while (tokens[index] != NULL)
    {
        if (strcmp(tokens[0],"unalias")==0||strcmp(tokens[0],"alias")==0)
        {
            return tokens;
        }
        if (getvalue(tokens[index],aliases)!=NULL) {
            char copy[MAX_INPUT_LENGTH];

            strcpy(copy, getvalue(tokens[index], aliases));

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

    return tokeniseString(final);
}

//
// Created by Balazs Szalay on 2021. 02. 11.
//
// header for our inbuilt,non-linux commands:cd,exit,getpath,setpath
// further dev: Create your inbuilt function somewhere here, then add it in the apply command.h
// you have to add its keyword to the inbuilt array AND you have to store its pointer in the function
// pointers array at the corresponding index
//
//

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

int exit1(char** tokens, char** history __attribute__((unused)), const int* front __attribute__((unused)), const int* rear __attribute__((unused)))
{
    free_tokens(tokens);
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

//
// Contains methods for handling the command history,
// including methods to store, print and execute previous commands.
// Created by Mark Oppo, Regina Gyimesi, Zoltan Kiss and Eamonn McClay 10/02/2021
//
int number_of_elements_in_history(char* history[]){

    if(history == NULL){
        return 0;
    }

    int i;
    for(i = 0; i < HISTORY_SIZE && history[i] != NULL && strcmp(history[i], "") != 0;){
        ++i;
    }

    return i;
}

void make_history(char** dest) {

    for (int k = 0; k < HISTORY_SIZE; k++) {
        dest[k] = (char *) malloc(sizeof(char) * MAX_INPUT_LENGTH);
        memset(dest[k], '\0', MAX_INPUT_LENGTH);
    }
}

void free_history(char** dest) {
    for (int k = 0; k < HISTORY_SIZE; k++) {
        free(dest[k]);
    }
    dest = NULL;
}

int is_history_empty(char** history){
    int i;
    for(i = 0; i < HISTORY_SIZE; ++i){
        if(*history[i] != '\0'){
            return FALSE;
        }
    }

    return TRUE;
}

int is_history_full(const int* front, const int *rear){
    // The array is full if:
    //      - the rear is the last available position
    //      - the rear and the front%HISTORY_SIZE are the same, the two flags are at the same position in the array
    if ((*front == 0 && *rear == HISTORY_SIZE-1) || (*rear == (*front-1)%(HISTORY_SIZE-1))){
        return TRUE;
    }
    return FALSE;
}

int add_to_history(char* input, char* history[], int* front, int *rear) {

    if(input == NULL || strcmp(input, "\n") == 0 || strcmp(input, "\0") == 0){
        fprintf(stderr, "Input error, not adding to history\n");
        return ERROR;
    }

    if (is_history_full(front, rear)){  // in a queue we wouldn't overwrite values, but here it can be done
        *front += 1;
        if(*front == HISTORY_SIZE){     // the front reached the end of the array, so we reset it to the first
            *front = 0;
        }
    }

    if(*front == -1){                                   // First element
        *front = 0;
        *rear = 0;
        memset(history[*rear], '\0', MAX_INPUT_LENGTH);
        strcpy(history[*rear], input);
    }
    else if(*rear == HISTORY_SIZE-1 && *front != 0){    // Rear reached the end, reset to the first element
        *rear = 0;
        memset(history[*rear], '\0', MAX_INPUT_LENGTH);
        strcpy(history[*rear], input);
    }
    else{                                               // Rear is somewhere between the first and last position
        *rear += 1;
        memset(history[*rear], '\0', MAX_INPUT_LENGTH);
        strcpy(history[*rear], input);
    }

    if(history[*rear] == NULL){
        fprintf(stderr, "Nothing has been added to the history!");
        return ERROR;
    }

    return TRUE;
}

char** exec_number_history(int number, char* history[], const int *front) {

    int temp = 0;
    int i;

    number--;       // Decreasing to make it equal to array index representation

    if (number >= 0 && number <= (HISTORY_SIZE-1)) {        // Checking if number is between the available number of commands

        for(i = *front; temp < number; ++i){                // From here we need to find the index we are looking for
            if(i == HISTORY_SIZE){                          // Because 'i' is different then the actual order, we have to match it
                i = 0;
            }
            temp++;
        }

        if(i == HISTORY_SIZE){                              // This is a edge case, because the loop, does not set i to
            i = 0;                                          // 0 if, temp becomes equal to the number and we reached the end of the array
        }

        if(i == -1){                              // This is a edge case, because the loop, does not set i to
            i = 0;                                          // 0 if, temp becomes equal to the number and we reached the end of the array
        }

        if(*history[i] != '\0'){                            // Copying and tokenizing the history command
            char line[MAX_INPUT_LENGTH] = {'\0'};
            memset(line, '\0', MAX_INPUT_LENGTH);
            strcpy(line, history[i]);
            return tokeniseString(line);
        }
    }

    fprintf(stderr, "History cannot be executed\n");
    if(number >= number_of_elements_in_history(history)){
        fprintf(stderr, "Number is greater, than the number of history elements.\n");
    }
    return NULL;
}

char* check_for_minus1(AList l)
{
    Alias* curr=*l;
    if(curr==NULL)
    {
        return NULL;
    }
    else if(strcmp(curr->value,"!-1")==0) //if first element is what we're looking for
    {
        return curr->key;
    }
    while(curr->next!=NULL) //iterate to the end of the list
    {
        curr=curr->next;

        if(strcmp(curr->value,"!-1")==0) //if first element is what we're looking for
        {
            return curr->key;
        }
    }

    return NULL;
}

char** exec_minus_number_history(int number, char* history[], int *last, AList aliases) {

    int temp = 0;
    int i;

    char* key_for_minus1 = check_for_minus1(aliases);
    char* super_temp = malloc(sizeof(char)*MAX_INPUT_LENGTH);
    if(key_for_minus1 != NULL){
        strcpy(super_temp, key_for_minus1);
        strcat(super_temp, "\n");
    }

    number--;       // Decreasing to make it equal to array index representation

    if (number >= 0 && number <= (HISTORY_SIZE-1)) {        // Checking if number is between the available number of commands

        for(i = *last; temp < number; --i){                // From here we need to find the index we are looking for
            if(i == -1){                          // Because 'i' is different then the actual order, we have to match it
                i = HISTORY_SIZE-1;
            }
            temp++;
        }

        if(i == -1){                              // This is a edge case, because the loop, does not set i to
            i = HISTORY_SIZE-1;                   // 0 if, temp becomes equal to the number and we reached the end of the array
        }

        if(*history[i] != '\0'){                            // Copying and tokenizing the history command
            char line[MAX_INPUT_LENGTH] = {'\0'};

            if(super_temp != NULL && strcmp(history[i], super_temp) == 0){
                strcpy(line, history[i-1]);
            }
            else{
                strcpy(line, history[i]);
            }

            free(super_temp);
            return tokeniseString(line);
        }
    }

    fprintf(stderr, "History cannot be executed\n");
    if(number >= number_of_elements_in_history(history)){
        fprintf(stderr, "Number is greater, than the number of history elements.\n");
    }
    free(super_temp);
    return NULL;
}

/*
 * Returns the number from the !<no.> text.
 */
int convert_number_to_int(const char* text){

    int number = 0;
    int offset = '0';   // offset between ASCII value of number and actual number

    for (int i = 1; text[i] != '\0'; i++) {
        if (text[i] == '-' && i == 1) continue;
        if (text[i] >= '0' && text[i] <= '9') {
            number = (number * 10) + (text[i] - offset);
            if (number < 0) return ERROR;    // overflow protection
        }
        else {
            fprintf(stderr, "Invalid invocation of history. Use case: ![!][-][1-20]\n");
            return ERROR;
        }
    }

    return number;
}

/*
* Executes the most recent command in history
*/
char** exec_recent_history(char* history[],int *last, AList aliases) {
    if(*(history[0]) != '\0') {
        return exec_minus_number_history(1, history, last, aliases);
    }

    fprintf(stderr, "No commands stored in recent history\n");
    return NULL;
}

/*
* Prints all commands stored in history
*/
int print_history(char** tokens, char* history[], const int* front, const int* rear) {

    int number_printed = 0;         // for bug fixing

    if(history == NULL || *history == NULL || tokens[1] != NULL){
        fprintf(stderr, "Invalid invocation of history. Use case: ![!][-][1-20]\n");

        free_tokens(tokens);

        return ERROR;
    }

    if(!is_history_empty(history)){
        int i;
        int index = 1;
        for(i = *front; i != *rear; ++i){
            if(i == HISTORY_SIZE){
                i = 0;
            }

            if(number_printed == HISTORY_SIZE){
                free_tokens(tokens);
                return TRUE;
            }

            printf("%d. %s\n", index, history[i]);
            ++index;

            number_printed++;
        }

        if(number_printed<HISTORY_SIZE){
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



/*
 * Function which decides what history command call
 */
char** check_history_type(char** tokens, char** history, int* front, int* last, AList aliases){
    char first_token[10];
    strcpy(first_token, tokens[0]);                         // storing the first token as single string

    if(first_token[0] == '!'){                                  // checking string chars one-by-one
        if(first_token[1] == '!' && first_token[2] == '\0'){

            free_tokens(tokens);

            return exec_recent_history(history,last, aliases);
        }
        else if(first_token[1] == '-'){
            int number = convert_number_to_int(tokens[0]);

            free_tokens(tokens);

            return exec_minus_number_history(number, history, last, aliases);

        }
        else if(first_token[1] >= '0' && first_token[1] <= '9'){
            int number = convert_number_to_int(tokens[0]);

            free_tokens(tokens);

            return  exec_number_history(number, history, front);
        }
        else if(strcmp(first_token, "!clear") == 0){
            free_history(history);
            make_history(history);
            *front = -1;
            *last = -1;
            printf("History has been reset!\n");

            free_tokens(tokens);

            return NULL;
        }
        else{
            fprintf(stderr, "Invalid invocation of history. Use case: ![!][-][1-20]\n");

            free_tokens(tokens);

            return NULL;
        }

    }

    return tokens;


}

/*
 * Creates or opens .hist_list file, saves every line of history to it and closes
 * when there is no more to write.
 */
int save_history(char* history[], const int* front, const int* rear) {
    int number_printed = 0;         // for bug fixing
    int i;

    char *filepath = malloc(sizeof(char) * MAX_INPUT_LENGTH);
    strcpy(filepath, getenv("HOME"));
    strcat(filepath, "/.hist_list");
    FILE *file = fopen(filepath, "w+");

    for(i = *front; i != *rear; ++i){
        if(i == HISTORY_SIZE){
            i = 0;
        }

        if(number_printed == HISTORY_SIZE){
            fclose(file);
            free(filepath);
            return TRUE;
        }

        if(strcmp(history[i], "") != 0 && strcmp(history[i], "\n") != 0 && strcmp(history[i], "\0") != 0){
            fprintf(file, "%s\n", history[i]);
        }
        else{
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



/*
 * Loads history from .hist_list file in home directory
 */
int load_history(char* history[],int* front, int* rear) {

    char *filepath = malloc(sizeof(char) * MAX_INPUT_LENGTH);
    strcpy(filepath, getenv("HOME"));
    strcat(filepath, "/.hist_list");
    FILE *file = fopen(filepath, "r+");
    char line[MAX_INPUT_LENGTH];

    if(!file) {
        fprintf(stderr, "History file could not be located\n");

        free(filepath);
        return ERROR;
    }
    else {
        while(fgets(line, sizeof(line), file)) {
            strtok(line, "\n");
            add_to_history(line, history, front, rear);
        }
    }

    fclose(file);
    free(filepath);
    return TRUE;
}

/*
// taking an array of tokenized inputs
// executing a command
// Created by Balázs Szalay on 2021. 02. 01.
*/
int apply_command(char** tokens, char** history,int* front, int* rear, AList aliases) {

    char *builtin_str[] = {
            "exit",
            "getpath",
            "setpath",
            "cd",
            "history"
    };

    int (*builtin_func[]) (char **, char**, const int*, const int*) = {
            &exit1,
            &getpath,
            &setpath,
            &change_directory,
            &print_history
    };

    if (tokens == NULL){   //if the first input is null or a NULL char, then we return 0 thus indicating it is an exit
        return FALSE;
    }

    if(*tokens == NULL){ //this happens if the user only pressed Enter, and nothing else as input
        free(tokens);
        return ERROR;
    }

    if(*tokens[0] == '\0'){ //if the first input is null or a NULL char, then we return 0 thus indicating it is an exit
        free(tokens);
        return FALSE;
    }
    else
    {

        char* history_line = malloc(sizeof(char) * MAX_INPUT_LENGTH);
        memset(history_line, '\0', MAX_INPUT_LENGTH);
        detokenize(tokens, history_line);

        for(int i = 0; i < 20 && tokens != NULL; ++i){
            tokens=substituteAlias(tokens,aliases);
        }

        if (tokens[0] != NULL && *(tokens[0]) != '\0' && *(tokens[0]) != '\n' && *(tokens[0]) != '!')
            add_to_history(history_line, history, front, rear);
        free(history_line);

        if(tokens != NULL && (*tokens)[0] == '!'){
            tokens = check_history_type(tokens, history, front, rear, aliases);
        }

        for(int i = 0; i < 20 && tokens != NULL; ++i){
            tokens=substituteAlias(tokens,aliases);
        }

        if(tokens != NULL && (*tokens)[0] == '!'){
            tokens = check_history_type(tokens, history, front, rear, aliases);
        }

        if(tokens != NULL && (strcmp(tokens[0],"alias") == 0 || strcmp(tokens[0],"unalias") == 0)){
            tokens = check_alias(tokens,aliases);
        }

        if(tokens!=NULL){

            for (int i = 0; i < COMMANDS_LENGTH; ++i) {
                if (strcmp(tokens[0], builtin_str[i]) == 0) //checking if the input is an inbuilt function and if so calling it
                    return (*builtin_func[i])(tokens, history, front, rear);                         // with the arguments provided with it
            }

            //else creating a Unix call and passing in the tokenized the arguments

            pid_t pid;
            pid=fork();

            if(pid<0)
            {
                printf("Fork failed");
                free_tokens(tokens);
                return TRUE;
            }
            else if(pid==0)
            {
                char temp[MAX_INPUT_LENGTH] = {'\0'};
                strcpy(temp, tokens[0]);

                execvp(temp,tokens);
                fprintf(stderr, "%s", tokens[0]);
                perror(" is not a valid command");

                free_tokens(tokens);
                exit(EXIT_FAILURE);
            }
            else{
                wait(NULL);
                free_tokens(tokens);
                return TRUE;
            }
        }

    }

    return TRUE;

}

//
// Gets the HOME directory and sets it. Checks if it has been correctly set.
// Created by Regina Gyimesi on 09/02/2021.
//

// gets and sets directory to HOME, if home directory is null then exists
int set_directory() {
    if (chdir(getenv("HOME")) != 0) {
        perror("Failed");
    }

    // checks if current directory is set to home
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }
    return 0;
}

//
// Contains methods to display a welcome message, print the command prompt, and read user input.
// Created by Eamonn McClay on 28/01/2021.
//
// Prints the prompt for the user to enter a command.
int print_prompt() {
    printf("> ");
    return 0;
}

// Reads the users input as a string.
char* read_input() {
    //static char input[514];
    char* input = malloc(sizeof(char)*MAX_INPUT_LENGTH);
    fgets(input, MAX_INPUT_LENGTH, stdin);

    if(input==NULL || *input == '\0')		//Ctr-D detected
    {
        return NULL;
    }

    if(strlen(input)==1)		//empty input
    {
        fflush(stdin);
        return input;			//does not actually return anything just calls itself until there is either a valid input or CTRL-d
    }

    if(input[strlen(input)-1]!='\n' && strlen(input)<MAX_INPUT_LENGTH-1){
        return NULL;
    }

    if(input[strlen(input)-1]!='\n' && strlen(input)>=MAX_INPUT_LENGTH-1)
    {
        printf("Input too long\n");
        fflush(stdin);
        memset(input, '\0', sizeof(char)*514);
        input[0] = '\n';
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
        return input;
    }

    return input;

}


//
// Created by Zoltan Kiss on 27/01/2021.
//
int main()
{
    char *line;
    char **args;
    int status;
    char *start_path = getenv("PATH"); // initial path stored
    char* history[20];
    AList aliases;
    int front, rear;
    front = rear = -1;

    aliases=new_list();
    set_directory();
    make_history(history);
    load_history(history, &front, &rear);
    load_aliases(aliases);

    do {

        print_prompt();
        line = read_input();

//        if (line != NULL && line[0] != '\0' && line[0] != '\n' && line[0] != '!')
//            add_to_history(line, history, &front, &rear);

        args = tokeniseString(line);

        status = apply_command(args, history,&front, &rear,aliases);

        free(line);
    } while (status);

    save_aliases(aliases);
    save_history(history, &front, &rear);
    free_history(history); // later on, the history will be saved in a separate file
    free_aliases(aliases);
    setenv("PATH", start_path, 1); // initial path restored
    printf("Path is restored to %s\n",getenv("PATH"));

    return 0;
}