//
// Contains methods for handling the command history,
// including methods to store, print and execute previous commands.
// Created by Mark Oppo, Regina Gyimesi and Zoltan Kiss 10/02/2021
//

#ifndef CS210_SIMPLE_SHELL_HISTORY_HANDLER_H
#define CS210_SIMPLE_SHELL_HISTORY_HANDLER_H

#endif //CS210_SIMPLE_SHELL_HISTORY_HANDLER_H

void make_history(char** dest) {
    
    for (int k = 0; k < HISTORY_SIZE; k++) {
        dest[k] = malloc(sizeof(char) * 514);
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

    free(input);

    return TRUE;
}

int exec_number_history(int number, char* history[]) {

    number--;
    if (number >= 0 && number <= (HISTORY_SIZE-1) && *(history[number]) != '\0') {
        char** tokens = tokenise(history[number]);
        return apply_command(tokens, history);
    }

    fprintf(stderr, "History cannot be executed\n");
    return ERROR;
}

int exec_minus_number_history(int number, char* history[]) {

    int current = 0;
    while (history[current] != NULL) {
        current++;
    }
    current = current - number;
    
    if (current >= 0 && current <= (HISTORY_SIZE-1) && *(history[current]) != '\0') {
        char** tokens = tokenise(history[current]);
        return apply_command(tokens, history);
    }

    fprintf(stderr, "History cannot be executed\n");
    return ERROR;
}

/*
 * Creates a single string by combining tokens
 */
char* create_input_from_tokens(char** tokens){

    int i = 1;                                          // count the number of tokens
    while(tokens[i++] != NULL){}

    char* command = malloc(sizeof(char*) * i);     // allocate memory accordingly
    if(tokens[1] != NULL){                              // if there are tokens initialize the string with the first token
        strcpy(command, tokens[1]);
        strcat(command, " ");
    }

    i = 2;
    while(tokens[i] != NULL){                           // combine the rest of the tokens to the string
        if(tokens[i+1] == NULL){
            strcat(command, tokens[i]);
        }
        else{
            strcat(command, tokens[i]);
            strcat(command, " ");
        }
        i++;
    }

    return command;
}

/*
 * Returns the number from the !<no.> text.
 */
int convert_number_to_int(char* text){
    char pure_number_chars[NUMBER_OF_DECIMALS] = {'\0'};    // initialize sanitized array
    int flag = 0;

    for(int i = 0; text[i] != '\0'; ++i){                   // fill array with numbers
        if(text[i]>='0' && text[i]<='9'){
            pure_number_chars[flag++] = text[i];
        }
    }

    for(int i = 0; i < NUMBER_OF_DECIMALS; ++i){            // check that the array only contains numbers and there is no overflow
        if((pure_number_chars[i] < '0' && pure_number_chars[i]!='\0') || (pure_number_chars[i] > '9' && pure_number_chars[i]!='\0') || (i == (NUMBER_OF_DECIMALS-1) && pure_number_chars[i]!='\0')){
            fprintf(stderr, "Something went wrong during number conversion\n");
            return ERROR;
        }
    }


    return atoi(pure_number_chars);
}

/*
 * Function which decides what history command call
 */
int check_history_type(char** tokens, char** history){
    char first_token[10];
    strcpy(first_token, tokens[0]);                         // storing the first token as single string

    if(first_token[0] == '!'){                                  // checking string chars one-by-one
        if(first_token[1] == '!' && first_token[2] == '\0'){
            return exec_recent_history();
        }
        else if(first_token[1] == '-'){
            int number = convert_number_to_int(tokens[0]);
            return exec_minus_number_history(number, history);
        }
        else if(first_token[1] >= '0' && first_token[1] <= '9'){
            int number = convert_number_to_int(tokens[0]);
            return exec_number_history(number, history);
        }
        else if(first_token[1] == '\0'){
            char* input = create_input_from_tokens(tokens);
            return add_to_history(input, history);
        }
        else if(strcmp(first_token, "!clear") == 0){
            free_history(history);
            make_history(history);
            printf("History has been reset!\n");
        }
        else{
            fprintf(stderr, "Invalid invocation of history. Use case: ![!][-][1-20]\n");
        }

    }

    return ERROR;


}

/*
* Executes the most recent command in history
*/
int exec_recent_history(char* history[]) {
    if(history[0] != NULL) {
        return exec_number_history(HISTORY_SIZE-1, history);
    }

    fprintf(stderr, "No commands stored in recent history\n");
    return ERROR;
}

/*
* Prints all commands stored in history
*/
int print_history(char* history[]) {
    if(history[0] != NULL) {
        int current = 0;
        while (history[current] != NULL) {
            printf("%d. %s\n", current+1, history[current]);
            current++;
        }
        return TRUE;
    }

    fprintf(stderr, "No commands stored in history\n");
    return ERROR;
}
