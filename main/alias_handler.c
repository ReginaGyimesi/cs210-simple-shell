/**
 * Contains methods for handling the aliases, including methods to add, print and remove aliases.
 *
 * @authors Balazs Szalay, Eamonn McClay, Regina Gyimesi, Zoltan Kiss
 * @date 26/02/2021
 */


#include "main.h"

AList aliases;

/**
 * Initialises a list for storing aliases
 */
void make_aliases()
{
    aliases = malloc(sizeof(Alias*));
    *aliases = NULL;
}

/**
 * Returns the list of aliases
 */
Alias* get_aliases() {
    return *aliases;
}

/**
 * Method for creating a single line of string from arrays of strings
 * @param tokens    arrays of string
 * @param value     variable where the string should be stored
 * @return          pointer to the string that has been created
 */
char* detokenize(char** tokens, char* value){
    int i;
    for(i = 0; tokens[i] != NULL && strcmp(tokens[i], "") != 0; ++i){
        char* temp = tokens[i];

        if(value[0] == '\0'){
            strcpy(value, temp);
        }
        else{
            strcat(value, " ");
            strcat(value, temp);
        }

    }

    strcat(value, "\0");

    return value;
}

/**
 * Creates a new alias list element
 * @param key       key for the alias
 * @param value     value that the alias will be representing
 * @return          new alias element
 */
Alias *new_alias(char* key, char*value){
    
    Alias* temp = (Alias*) malloc(sizeof(Alias));
    temp->key = malloc(sizeof (char) * MAX_INPUT_LENGTH);
    temp->value = malloc(sizeof (char) * MAX_INPUT_LENGTH);

    memset(temp->key, '\0', MAX_INPUT_LENGTH);
    memset(temp->value, '\0', MAX_INPUT_LENGTH);

    strcpy(temp->key, key);
    strcpy(temp->value, value);
    temp->next = NULL;
    return temp;
}

/**
 * Adds a new alias to the aliases list
 * @param key       key for the alias
 * @param value     value that the alias will be representing
 */
void add_alias(char* key, char* value)
{

    Alias* newN = new_alias(key,value);

    Alias* current = *aliases;
    if(current == NULL)
    {
        *aliases = newN;
    }
    else {
        newN->next = current;
        *aliases = newN;
    }

}

/**
 * Prints out the currently set aliases
 */
void print_alias()
{

    Alias* curr = *aliases;
    if(curr == NULL || *aliases == NULL)
    {
        printf("No aliases have been set\n");
        return;
    }

    while(curr!=NULL)
    {
        printf("%s %s\n",curr->key,curr->value);
        curr=curr->next;
    }

}

/**
 * If a new alias key is already present in the list, this method replaces
 * the value of the old alias with the new value
 * @param key       aliases key
 * @param tokens    value that it will be replaced with
 * @return          TRUE if alias with such key was present
 *                  ERROR otherwise
 */
int replace_if_exists(char* key, char** tokens)
{

    Alias* current = *aliases;
    while(current!=NULL)
    {
        if(strcmp(current->key,key)==0)
        {
            memset(current->value, '\0', MAX_INPUT_LENGTH);

            char value[MAX_INPUT_LENGTH] = {'\0'};

            detokenize(tokens, value);

            strcpy(current->value, value);

            printf("Alias was replaced\n");

            return TRUE;

        }
        else
        {
            current=current->next;
        }

    }
    return ERROR;

}

/**
 * This method handles adding and replacing aliases.
 * @param key           key for alias
 * @param tokens        value of the new alias
 * @return              TRUE if alias with such key was present
 *                      ERROR it is a brand new alias
 */
int add_replace(char* key, char** tokens)
{
    int status = replace_if_exists(key, tokens);
    if(status == ERROR)
    {
        char value[MAX_INPUT_LENGTH];
        memset(value, '\0', MAX_INPUT_LENGTH);
        detokenize(tokens, (char *) value);
        add_alias(key, (char *) value);
        return TRUE;
    }
    return status;

}

/**
 * Deletes a alias with the specified key
 * @param key       the alias key that will be deleted
 * @return          TRUE, if alias was deleted successfully
 *                  ERROR, if no such alias is present or no aliases have been set
 */
int delete_alias(char* key)
{

    Alias* curr = *aliases;
    Alias* previous = NULL;
    if(curr == NULL)
    {
        printf("No aliases have been set\n"); //no aliases have been set invalid operation
        return ERROR;
    }
    else if(strcmp(curr->key,key)==0) //if first element is what we're looking for
    {
        *aliases = curr->next;

        free(curr->key);
        free(curr->value);
        curr->value = NULL;
        free(curr);
        return TRUE;
    }
    while(curr->next!=NULL) //iterate to the end of the list
    {
        previous=curr;
        curr=curr->next;

        if(strcmp(curr->key,key)==0)
        {
            if(curr->next!=NULL)
            {
                previous->next=curr->next;

                free(curr->key);
                free(curr->value);
                free(curr);
                return TRUE;
            }
            else{
                previous->next=NULL;

                free(curr->key);
                free(curr->value);
                free(curr);
                return TRUE;
            }

        }

    }

    printf("Alias with these parameters was not found\n");
    return ERROR;
}

/**
 * This method handles the possible alias commands
 * @param tokens        user input string tokenized
 * @return              tokens of the alias value or NULL if something went wrong
 */
char** check_alias(char** tokens) {

    if(tokens == NULL || *tokens == NULL){      // If somehow the aliases list does not exist or no aliases present
        fprintf(stderr, "Invalid tokens or no aliases\n");
        free_tokens(tokens);
        return NULL;
    }

    if(tokens[1] != NULL && tokens[2] != NULL && strcmp(tokens[2],"")==0 && strcmp(tokens[2],"alias")==0){  // If something bad happens during tokenizing
        fprintf(stderr, "Invalid tokens or no aliases\n");
        free_tokens(tokens);
        return NULL;
    }

    // If alias does not have the correct number of parameters
    if(strcmp(tokens[0],"alias")==0 && tokens[1]!=NULL && strcmp(tokens[1], "") != 0 && (tokens[2]==NULL || strcmp(tokens[2], "") == 0)){
        fprintf(stderr, "Alias needs 2 parameters, only 1 was given\n");
        free_tokens(tokens);
        return NULL;
    }

    // If the user wants add or replace a new alias
    if(strcmp(tokens[0],"alias")==0 && tokens[1]!=NULL && tokens[2]!=NULL)
    {
        char key_temp[MAX_INPUT_LENGTH];
        memset(key_temp, '\0', MAX_INPUT_LENGTH);
        strcpy(key_temp, tokens[1]);

        add_replace(key_temp, &tokens[2]);

        free_tokens(tokens);
        return NULL;

    }
    // If the user wants to print out the aliases
    else if(strcmp(tokens[0],"alias")==0 && (tokens[1]==NULL || (strcmp(tokens[1], "") == 0)))
    {
        print_alias();
        free_tokens(tokens);
        return NULL;
    }
    // If the user wants to unalias a alias
    else if (strcmp(tokens[0],"unalias")==0 && tokens[1]!=NULL && tokens[2]==NULL)
    {
        delete_alias(tokens[1]);
        free_tokens(tokens);
        return NULL;
    }
    // None of the above
    else{
        free_tokens(tokens);
        return NULL;
    }

}


/**
 * Creates or opens .aliases file, saves every line of alias with the corresponding key and closes
 * when there is no more to write.
 * @return  TRUE, if save was successful
 */
int save_aliases() {
    Alias* current = *aliases;
    char* filepath = malloc(sizeof(char) * MAX_INPUT_LENGTH);
    memset(filepath, '\0', MAX_INPUT_LENGTH);

    strcpy(filepath, getenv("HOME"));
    strcat(filepath, "/.aliases");
    FILE* file = fopen(filepath, "w+");

    while(current!=NULL){
        char temp_key[MAX_INPUT_LENGTH];
        char temp_value[MAX_INPUT_LENGTH];

        memset(temp_key, '\0', MAX_INPUT_LENGTH);
        memset(temp_value, '\0', MAX_INPUT_LENGTH);

        strcpy(temp_key, current->key);
        strcpy(temp_value, current->value);

        fprintf(file, "%s %s\n", temp_key, temp_value); // writes key and the corresponding alias to file
        current = current->next;
    }

    fclose(file);
    free(filepath);
    return TRUE;
}

/**
 * This method deletes aliases and frees up the allocated memory
 * @return      TRUE if everything went alright
 *              ERROR if aliases list does not exist
 */
int free_aliases(){
    Alias* current = *aliases;
    Alias* prev = NULL;

    if(current==NULL)
    {
        return ERROR;
    }

    while(current!=NULL){
        prev = current;
        current = current->next;

        free(prev->key);
        prev->key = NULL;
        free(prev->value);
        prev->value = NULL;
        free(prev);
    }

    *aliases = NULL;

    return TRUE;
}

/**
 * Opens .aliases file, reads in every line and adds to alias list.
 * @return  TRUE, if load was successful
 *          ERROR, if alias save file was corrupted or does not exist
 */
int load_aliases() {
    char* filepath = malloc(sizeof(char) * MAX_INPUT_LENGTH);
    memset(filepath, '\0', MAX_INPUT_LENGTH);
    strcpy(filepath, getenv("HOME"));
    strcat(filepath, "/.aliases");
    FILE* file = fopen(filepath, "r+");

    char line[MAX_INPUT_LENGTH];
    char temp[MAX_INPUT_LENGTH];
    char* key;
    char* value;

    memset(line, '\0', MAX_INPUT_LENGTH);
    memset(temp, '\0', MAX_INPUT_LENGTH);

    if(!file) {
        fprintf(stderr, "Aliases file could not be located\n");

        free(filepath);
        return ERROR;
    } // Returns error if .aliases file not located
    else {
        while(fgets(line, sizeof(line), file)) {

            int i;
            for(i = 0; i < strcspn(line, "\n"); ++i){
                temp[i] = line[i];
            }
            temp[i] = '\0';

            key = strtok(temp, " ");
            if(key == NULL){
                fprintf(stderr, "Alias file is corrupt, file read stopped and aliases deleted.\n");
                free_aliases();
                aliases = NULL;
                make_aliases();
                free(filepath);
                fclose(file);
                return ERROR;
            }
            value = temp + strlen(key) + 1;
            strcat(value, "\0");

            add_alias(key, value);
        }
    } // Other wise adds alias grabbed from each line

    fclose(file);
    free(filepath);
    return TRUE;
}

/**
 * Get the alias value for a key
 * @param key   alias key to be searched for
 * @return      value of alias
 */
char* getvalue(char* key)
{
    Alias* current = *aliases;
    if (current==NULL)
    {
        return NULL;
    }
    else
    {
        while (current!=NULL)
        {
            if (strcmp(key,current->key)==0)
            {
                return current->value;
            }
            current=current->next;
        }
        return NULL;
    }


}

/**
 * This method replaces a input string with the aliased version, if such string part found
 * @param tokens    user input string
 * @return          new substituted string
 */
char** substituteAlias(char** tokens)
{
    char temp[MAX_INPUT_LENGTH];
    int index = 0;
    while (tokens[index] != NULL)
    {
        if (strcmp(tokens[0],"unalias")==0||strcmp(tokens[0],"alias")==0)
        {
            return tokens;
        }
        if (getvalue(tokens[index])!= NULL) {
            char copy[MAX_INPUT_LENGTH];

            strcpy(copy, getvalue(tokens[index]));

            if(index == 0) {
                strcpy(temp, copy);
            }
            else{
                strcat(temp, " ");
                strcat(temp, copy);
            }

        }
        else{
            char copy[MAX_INPUT_LENGTH];

            strcpy(copy, tokens[index]);

            if(index == 0) {
                strcpy(temp, copy);
            }
            else{
                strcat(temp, " ");
                strcat(temp, copy);
            }

        }

        index++;
    }

    char final[MAX_INPUT_LENGTH];
    memset(final, '\0', MAX_INPUT_LENGTH);
    strcpy(final, temp);
    memset(temp, '\0', MAX_INPUT_LENGTH);

    int n_tokens;
    for(n_tokens = 0; tokens[n_tokens] != NULL; ++n_tokens){}

    for(int j = 0; j < n_tokens; ++j){
        free(tokens[j]);
    }

    free(tokens);

    return tokenise_string(final);
}