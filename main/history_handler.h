//
// Contains methods for handling the command history,
// including methods to store, print and execute previous commands.
// Created by Mark Oppo, Regina Gyimesi, Zoltan Kiss and Eamonn McClay 10/02/2021
//

#ifndef CS210_SIMPLE_SHELL_HISTORY_HANDLER_H
#define CS210_SIMPLE_SHELL_HISTORY_HANDLER_H

#endif //CS210_SIMPLE_SHELL_HISTORY_HANDLER_H

void make_history(char** dest) {
    
    for (int k = 0; k < HISTORY_SIZE; k++) {
        dest[k] = malloc(sizeof(char) * MAX_INPUT_LENGTH);
        dest[k][0] = '\0';
    }
}

void free_history(char** dest) {
    for (int k = 0; k < HISTORY_SIZE; k++) {
        free(dest[k]);
    }

    dest = NULL;
}

int add_to_history(char* input, char* history[]) {

    int i = 0;
    while (i < HISTORY_SIZE && *(history[i]) != '\0') {
        i++;
    }

    if (i == HISTORY_SIZE) {    // array is full
        char *temp = history[0];
        for (int j = 1; j <= (HISTORY_SIZE-1); j++) {
            history[j - 1] = history[j];
        }
        history[HISTORY_SIZE-1] = temp;
        strcpy(history[HISTORY_SIZE-1], input);
    } else {
        strcpy(history[i], input);
    }

    return TRUE;
}

char** exec_number_history(int number, char* history[]) {

    number--;
    if (number >= 0 && number <= (HISTORY_SIZE-1) && *(history[number]) != '\0') {
        char* line = malloc(sizeof(char) * MAX_INPUT_LENGTH);
        strcpy(line, history[number]);
        char** tokens = tokenise(line);
        return tokens;
    }

    fprintf(stderr, "History cannot be executed\n");
    return NULL;
}

char** exec_minus_number_history(int number, char* history[]) {

    int current = 0;
    while (current < HISTORY_SIZE && *(history[current]) != '\0') {
        current++;
    }
    current = current - number;
    
    if (current >= 0 && current <= (HISTORY_SIZE-1) && *(history[current]) != '\0') {
        char* line = malloc(sizeof(char) * MAX_INPUT_LENGTH);
        strcpy(line, history[current]);
        char** tokens = tokenise(line);
        return tokens;
    }

    fprintf(stderr, "History cannot be executed\n");
    return NULL;
}

/*
 * Returns the number from the !<no.> text.
 */
int convert_number_to_int(const char* text){
    char pure_number_chars[NUMBER_OF_DECIMALS] = {'\0'};    // initialize sanitized array
    int flag = 0;
    char** end_ptr = NULL;

    for(int i = 0; text[i] != '\0'; ++i){                   // fill array with numbers
        if(text[i] == '!' && i == 0) continue;
        if(text[i] == '-' && i == 1) continue;
        if((text[i]>='0' && text[i]<='9') || text[i] == '\0'){
            pure_number_chars[flag++] = text[i];
        }
        else{
            fprintf(stderr, "Something went wrong during number conversion\n");
            fprintf(stderr, "Invalid invocation of history. Use case: ![!][-][1-20]\n");
            return ERROR;
        }
    }

    for(int i = 0; i < NUMBER_OF_DECIMALS; ++i){            // check that the array only contains numbers and there is no overflow
        if((pure_number_chars[i] < '0' && pure_number_chars[i]!='\0') || (pure_number_chars[i] > '9') || (i == (NUMBER_OF_DECIMALS-1) && pure_number_chars[i]!='\0')){
            fprintf(stderr, "Something went wrong during number conversion\n");
            fprintf(stderr, "Invalid invocation of history. Use case: ![!][-][1-20]\n");
            return ERROR;
        }
    }


    return (int)strtol(pure_number_chars, end_ptr, 10);
}

/*
* Executes the most recent command in history
*/
char** exec_recent_history(char* history[]) {
    if(*(history[0]) != '\0') {
        return exec_minus_number_history(1, history);
    }

    fprintf(stderr, "No commands stored in recent history\n");
    return NULL;
}

/*
* Prints all commands stored in history
*/
int print_history(char** tokens, char* history[]) {
    if(history == NULL || *history == NULL || tokens[1] != NULL){
        fprintf(stderr, "Invalid invocation of history. Use case: ![!][-][1-20]\n");
        return ERROR;
    }

    if(*(history[0]) != '\0') {
        int current = 0;
        while (current < HISTORY_SIZE && *(history[current]) != '\0') {
            printf("%d. %s", current+1, history[current]);
            current++;
        }
        return TRUE;
    }

    fprintf(stderr, "No commands stored in history\n");
    fprintf(stderr, "Invalid invocation of history. Use case: ![!][-][1-20]\n");
    return ERROR;
}

/*
 * Function which decides what history command call
 */
char** check_history_type(char** tokens, char** history){
    char first_token[10];
    strcpy(first_token, tokens[0]);                         // storing the first token as single string

    if(first_token[0] == '!'){                                  // checking string chars one-by-one
        if(first_token[1] == '!' && first_token[2] == '\0'){
            return exec_recent_history(history);
        }
        else if(first_token[1] == '-'){
            int number = convert_number_to_int(tokens[0]);
            return exec_minus_number_history(number, history);
        }
        else if(first_token[1] >= '0' && first_token[1] <= '9'){
            int number = convert_number_to_int(tokens[0]);
            return exec_number_history(number, history);
        }
        else if(strcmp(first_token, "!clear") == 0){
            free_history(history);
            make_history(history);
            printf("History has been reset!\n");
            return NULL;
        }
        else{
            fprintf(stderr, "Invalid invocation of history. Use case: ![!][-][1-20]\n");
        }

    }

    return NULL;


}

/*
 * Loads history from .hist_list file in home directory
 */
int load_history(char* history[]) {
    char *filepath = strcat(getenv("HOME"), "/.hist_list");
    FILE *file = fopen(filepath, "r+");
    char line[MAX_INPUT_LENGTH];

    if(!file) {
        fprintf(stderr, "History file could not be located\n");
        return ERROR;
    }
    else {
        while(fgets(line, sizeof(line), file)) {
            add_to_history(line, history);
        }
    }

    fclose(file);
    
    return TRUE;
}
