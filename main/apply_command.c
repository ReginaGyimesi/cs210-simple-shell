/**
 * Taking an array of tokenized inputs and executing the proper command.
 *
 * @authors Balazs Szalay, Zoltan Kiss
 * @date 01/02/2021
 */

#include "main.h"

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