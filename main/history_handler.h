//
// Contains methods for handling the command history,
// including methods to store, print and execute previous commands.
// Created by Mark Oppo, Regina Gyimesi and (add other names) 10/02/2021
//

#ifndef CS210_SIMPLE_SHELL_HISTORY_HANDLER_H
#define CS210_SIMPLE_SHELL_HISTORY_HANDLER_H

#endif //CS210_SIMPLE_SHELL_INBUILT_H

void make_history(char** dest) {
    
    for (int k = 0; k < 20; k++) {
        dest[k] = malloc(sizeof(char) * 514);
        dest[k][0] = '\0';
    }
}

void free_history(char** dest) {
    for (int k = 0; k < 20; k++) {
        free(dest[k]);
    }

    dest = NULL;
}

int add_to_history(char* input, char* history[]) {

    int i = 0;
    while (i < 20 && *(history[i]) != '\0') {
        i++;
    }

    if (i == 20) {    // array is full
        char *temp = history[0];
        for (int j = 1; j <= 19; j++) {
            history[j - 1] = history[j];
        }
        history[19] = temp;
        strcpy(history[19], input);
    } else {
        strcpy(history[i], input);
    }

    return TRUE;
}

int exec_number_history(int number, char* history[]) {
    
    number--;
    if (number >= 0 && number <= 19 && *(history[number]) != '\0') {
        char** tokens = tokenise(history[number]);
        return apply_command(tokens, history);
    }

    perror("History cannot be executed");
    return FALSE;
}

int exec_minus_number_history(int number, char* history[]) {

    int current = 0;
    while (*(history[current]) != '\0') {
        current++;
    }
    current = current - number;
    
    if (current >= 0 && current <= 19 && *(history[current]) != '\0') {
        char** tokens = tokenise(history[current]);
        return apply_command(tokens, history);
    }

    perror("History cannot be executed");
    return FALSE;
}

char* create_input_from_tokens(char** tokens){
    int i = 1;
    while(tokens[i] != NULL){
        i++;
    }

    char* command = malloc(sizeof(char*) * i);
    if(tokens[1] != NULL){
        strcpy(command, tokens[1]);
        strcat(command, " ");
    }

    i = 2;
    while(tokens[i] != NULL){
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

int convert_number_to_int(char* text){
    char pure_number_chars[3];
    int flag = 0;
    for(int i = 0; text[i] != '\0'; ++i){
        if(text[i]>='0' && text[i]<='9'){
            pure_number_chars[flag++] = text[i];
        }
    }

    for(int i = 0; i < 3; ++i){
        if((pure_number_chars[i] < '0' && pure_number_chars[i]!='\0') || (pure_number_chars[i] > '9' && pure_number_chars[i]!='\0') || (i == 2 && pure_number_chars[i]!='\0')){
            fprintf(stderr, "Something went wrong during number conversion");
            return ERROR;
        }
    }

    return atoi(pure_number_chars);
}

int check_history_type(char** tokens, char** history){
    char first_token[6];
    strcpy(first_token, tokens[0]);

    if(first_token[0] == '!'){
        if(first_token[1] == '!' && first_token[2] == '\0'){
            return TRUE;
            //return exec_recent_history();
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
        else{
            fprintf(stderr, "Invalid invocation of history. Use case: ![!][-][1-20]");
        }

    }

    return ERROR;


}
