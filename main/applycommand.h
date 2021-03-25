/*
// taking an array of tokenized inputs
// executing a command
// Created by Balázs Szalay on 2021. 02. 01.
*/



#ifndef CS210_SIMPLE_SHELL_APPLYCOMMAND_H
#define CS210_SIMPLE_SHELL_APPLYCOMMAND_H

#endif //CS210_SIMPLE_SHELL_APPLYCOMMAND_H

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
