/**
 * Contains methods to print the command prompt, read user input and tokenise it.
 * @authors Eamonn McClay, Mark Oppo, Balazs Szalay
 * @date 28/01/2021
 */

#include "main.h"

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