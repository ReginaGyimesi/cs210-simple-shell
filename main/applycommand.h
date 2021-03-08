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
        // This is where or main command repetition happening*tokens = NULL;
        //                tokens = NULL;
        int one_of_history_or_alias = TRUE;                 // If this is false, that means the latest command wasnt a history or a alias command
        int limit = 0;                                      // Our maximum recurring limit
        while(one_of_history_or_alias && limit < 20){       // This is a O(n^2) function, where n is the length of the command

            /*
             * We are first checking for a history command.
             * So a history command can either return a new set of tokens or NULL
             * For example:
             *      !5 -> echo "Hello World!": in this case this whole loop would run only
             *                                 once, because `echo` is a Unix command and
             *                                 would exit by the one_of_history_or_alias = FALSE
             *                                 (this while loop will run once as well).
             *      !5 -> history:             same as before.
             *      !5 -> !9:                  a case that never happens, but if it does, this tiny
             *                                 while loop runs twice, and calls whatever !9 stands for.
             *      !5 -> alias:               whole loop and this loop runs once, exits as NULL after
             *                                 check_alias()
             *      !5 -> getpath:             (in-built command, same as a Unix command) whole loop and this loop runs twice, exits as
             *                                 one_of_history_or_alias = FALSE
             *      !5 -> iamgarbage:          this would return itself, and exits by one_of_history_or_alias = FALSE
             */
            while(tokens != NULL && limit < 20 && (*tokens)[0] == '!'){
                tokens = check_history_type(tokens, history, front, rear);
            }

            /*
             * Check if the history checker returned NULL (most possible case):
             *      !5 -> !clear
             */
            if(tokens == NULL){
                return TRUE;
            }

            /*
             * After we made sure that the token is not a history command anymore
             * We move on to the aliases. Here we check, if this command is a
             * alias setting command (but not the aliased command)
             * Alias setting commands always return NULL so here that is the only exit
             * condition. I have no idea, why did I put a while loop here, because we cannot
             * recursively check for the tokens, because every return is NULL.
             */
            while(tokens != NULL && limit < 20 && (strcmp(tokens[0],"alias") == 0 || strcmp(tokens[0],"unalias") == 0)){
                tokens = check_alias(tokens,aliases);
            }

            /*
             * If it was an alias setting command, then we return
             */
            if(tokens == NULL){
                return TRUE;
            }

            /*
             * This is where we check, that the command may be some aliased word.
             * So if we are here, we might establish, that the command is not a history invocation
             * nor an alias setting command. So it from now it can be 4 thing:
             *  - garbage
             *  - an aliased word
             *  - either an in-built or Unix command
             *  - (or it is actually a history or alias setting command,
             *    so we have to start the whole loop again)
             *
             * get_key_from_tokens() will return new tokens (the aliased command)
             * or the same tokens that were inputted. This also sets the
             * one_of_history_or_alias boolean to FALSE, if the tokens are NULL or no
             * such aliased command was found.
             */
            tokens = get_key_from_tokens(tokens, aliases, &one_of_history_or_alias);

            /*
             * If somehow the tokens becomes NULL, we just quit the loop.
             */
            if(tokens == NULL){
                one_of_history_or_alias = FALSE;
            }

            limit++;        // We increase our limit, to dont stuck in a infinite loop.

            /*
             * And we start the whole loop again. Either because the original tokens were
             * overwritten by something new or none of the methods here set a exit condition.
             *
             * If there is a exit condition, we move on with our life to either execute an in-built
             * command or a Unix-command (or neither), with the new set of tokens that were overwritten by
             * one of the methods.
             */
        }

        if(tokens!=NULL){
            for (int i = 0; i < COMMANDS_LENGTH; ++i) {
                if (strcmp(tokens[0], builtin_str[i]) == 0) //checking if the input is an inbuilt function and if so calling it
                    return (*builtin_func[i])(tokens, history, front, rear);                         // with the arguments provided with it
            }

            //check if input is in the aliases

            //else creating a Unix call and passing in the tokenized the arguments
            pid_t pid;
            pid=fork();
            if(pid<0)
            {
                printf("Fork failed");
                *tokens = NULL;
                tokens = NULL;
                return TRUE;
            }
            else if(pid==0)
            {
                execvp(tokens[0],tokens);
                perror("Error: ");
                *tokens = NULL;
                tokens = NULL;
                exit(EXIT_FAILURE);
            }
            else{
                wait(NULL);
                printf("Child complete\n");
                *tokens = NULL;
                tokens = NULL;
                return TRUE;
            }
        }

    }

    *tokens = NULL;
    tokens = NULL;
    return TRUE;

}
