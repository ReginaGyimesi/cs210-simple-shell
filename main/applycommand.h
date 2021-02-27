/*
// taking an array of tokenized inputs
// executing a command
// Created by Balázs Szalay on 2021. 02. 01.
*/



#ifndef CS210_SIMPLE_SHELL_APPLYCOMMAND_H
#define CS210_SIMPLE_SHELL_APPLYCOMMAND_H

#endif //CS210_SIMPLE_SHELL_APPLYCOMMAND_H

int apply_command(char** tokens, char** history,int* front, int* rear,AList aliases) {

    char *builtin_str[] = {
            "exit",
            "getpath",
            "setpath",
            "cd",
            "history"
    };

    int (*builtin_func[]) (char **, char**, int*, int*) = {
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
        return ERROR;
    }

    if(*tokens[0] == '\0'){ //if the first input is null or a NULL char, then we return 0 thus indicating it is an exit
        return FALSE;
    }
    else
    {
        if((*tokens)[0] == '!'){                        // check if history command
            tokens = check_history_type(tokens, history, front, rear);

        }
        if (strcmp(tokens[0],"alias")==0)
        {
            return check_alias(tokens,aliases);
        }


        if(tokens!=NULL){

            if(strcmp(tokens[0],"alias")==0||strcmp(tokens[0],"unalias")==0)
            {

            }
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
                return TRUE;
            }
            else if(pid==0)
            {
                execvp(tokens[0],tokens);
                perror("Error: ");
                exit(EXIT_FAILURE);
            }
            else{
                wait(NULL);
                printf("Child complete\n");
                return TRUE;
            }
        }



    }

    return TRUE;

}
