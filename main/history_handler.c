/**
 * Contains methods for handling the command history, including methods to store, print and execute previous commands.
 *
 * @authors Mark Oppo, Regina Gyimesi, Eamonn McClay, Zoltan Kiss
 * @date 10/02/2021
 */

#include "main.h"

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