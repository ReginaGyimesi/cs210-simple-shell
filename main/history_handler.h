//
// Contains methods for handling the command history,
// including methods to store, print and execute previous commands.
// Created by Mark Oppo, Regina Gyimesi, Zoltan Kiss and Eamonn McClay 10/02/2021
//

#ifndef CS210_SIMPLE_SHELL_HISTORY_HANDLER_H
#define CS210_SIMPLE_SHELL_HISTORY_HANDLER_H

#endif //CS210_SIMPLE_SHELL_HISTORY_HANDLER_H


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

        if(*history[i] != '\0'){                            // Copying and tokenizing the history command
            char line[MAX_INPUT_LENGTH] = {'\0'};
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

char** exec_minus_number_history(int number, char* history[], const int *last) {

    int temp = 0;
    int i;

    number--;       // Decreasing to make it equal to array index representation

    if (number >= 0 && number <= (HISTORY_SIZE-1)) {        // Checking if number is between the available number of commands

        for(i = *last; temp < number; --i){                // From here we need to find the index we are looking for
            if(i == -1){                          // Because 'i' is different then the actual order, we have to match it
                i = HISTORY_SIZE-1;
            }
            temp++;
        }

        if(i == -1){                              // This is a edge case, because the loop, does not set i to
            i = HISTORY_SIZE-1;                                          // 0 if, temp becomes equal to the number and we reached the end of the array
        }

        if(*history[i] != '\0'){                            // Copying and tokenizing the history command
            char line[MAX_INPUT_LENGTH] = {'\0'};
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
char** exec_recent_history(char* history[],int *last) {
    if(*(history[0]) != '\0') {
        return exec_minus_number_history(1, history,last);
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

            printf("%d. %s", index, history[i]);
            ++index;

            number_printed++;
        }

        if(number_printed<HISTORY_SIZE){
            printf("%d. %s", index, history[i]);
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
char** check_history_type(char** tokens, char** history, int* front, int* last){
    char first_token[10];
    strcpy(first_token, tokens[0]);                         // storing the first token as single string

    if(first_token[0] == '!'){                                  // checking string chars one-by-one
        if(first_token[1] == '!' && first_token[2] == '\0'){

            free_tokens(tokens);

            return exec_recent_history(history,last);
        }
        else if(first_token[1] == '-'){
            int number = convert_number_to_int(tokens[0]);

            free_tokens(tokens);

            return exec_minus_number_history(number, history, last);

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
            fprintf(file, "%s", history[i]);
        }
        else{
            number_printed++;
            continue;
        }

        number_printed++;
    }

    fprintf(file, "%s", history[i]);
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
            add_to_history(line, history, front, rear);
        }
    }

    fclose(file);
    free(filepath);
    return TRUE;
}

