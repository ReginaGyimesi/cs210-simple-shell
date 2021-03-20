//
// Contains methods for handling the aliases,
// including methods to add, print and remove aliases.
// Created by Balazs Szalay, Eamonn McClay and Regina Gyimesi on 2021. 02. 26.
//

#ifndef CS210_SIMPLE_SHELL_ALIAS_HANDLER_CLEAN_H
#define CS210_SIMPLE_SHELL_ALIAS_HANDLER_CLEAN_H

#endif //CS210_SIMPLE_SHELL_ALIAS_HANDLER_CLEAN_H

struct alias{
    char* key;
    char* value;
    struct alias *next;
};

typedef struct alias Alias;
typedef struct alias** AList;

AList new_list()
{
    AList temp=malloc(sizeof(Alias*));
    *temp=NULL;
    return temp;
}

void free_tokens(char** tokens){
    int n_tokens;
    for(n_tokens = 0; tokens[n_tokens] != NULL; ++n_tokens){}

    for(int j = 0; j < n_tokens; ++j){
        free(tokens[j]);
    }

    free(tokens);
}

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

Alias *new_alias(char* key,char*value){
    Alias *temp = (Alias*) malloc(sizeof(Alias));
    temp->key = malloc(sizeof (char) * MAX_INPUT_LENGTH);
    temp->value = malloc(sizeof (char) * MAX_INPUT_LENGTH);

    memset(temp->key, '\0', MAX_INPUT_LENGTH);
    memset(temp->value, '\0', MAX_INPUT_LENGTH);

    strcpy(temp->key, key);
    strcpy(temp->value, value);
    temp->next = NULL;
    return temp;
}

void add_alias(AList l,char*key,char*value)
{

    Alias* newN = new_alias(key,value);

    Alias* current = *l;
    if(current==NULL)
    {
        *l=newN;
    }
    else{
        newN->next=current;
        *l=newN;
    }

}

void print_alias(AList l)
{

    Alias *curr=*l;
    if(curr==NULL||*l==NULL)
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

int replace_if_exists(AList l, char* key, char** tokens)
{

    Alias *current=*l;
    while(current!=NULL)
    {
        if(strcmp(current->key,key)==0)
        {
            memset(current->value, '\0', MAX_INPUT_LENGTH);

            char value[MAX_INPUT_LENGTH] = {'\0'};

            detokenize(tokens, value);

            strcpy(current->value, value);

            printf("Alias was replaced\n");

            return 1;

        }
        else
        {
            current=current->next;
        }

    }
    return -1;

}

int add_replace(AList l,char*key, char** tokens)
{
    int status = replace_if_exists(l, key, tokens);
    if(status==-1)
    {
        char value[MAX_INPUT_LENGTH];
        memset(value, '\0', MAX_INPUT_LENGTH);
        detokenize(tokens, (char *) value);
        add_alias(l, key, (char *) value);
        return 1;
    }
    return status;

}

int delete_alias(AList l, char*key)
{

    Alias* curr=*l;
    Alias* previous=NULL;
    if(curr==NULL)
    {
        printf("No aliases have been set\n"); //no aliases have been set invalid operation
        return -1;
    }
    else if(strcmp(curr->key,key)==0) //if first element is what we're looking for
    {
        *l = curr->next;

        free(curr->key);
        free(curr->value);
        curr->value = NULL;
        free(curr);
        return 1;
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
                return 1;
            }
            else{
                previous->next=NULL;

                free(curr->key);
                free(curr->value);
                free(curr);
                return 1;
            }

        }

    }

    printf("Alias with these parameters was not found\n");
    return -1;
}

char** check_alias(char** tokens, AList aliases) {

    if(tokens == NULL || *tokens == NULL){
        fprintf(stderr, "Invalid tokens or no aliases\n");
        free_tokens(tokens);
        return NULL;
    }

    if(tokens[1] != NULL && tokens[2] != NULL && strcmp(tokens[2],"")==0 && strcmp(tokens[2],"alias")==0){
        fprintf(stderr, "Invalid tokens or no aliases\n");
        free_tokens(tokens);
        return NULL;
    }

    if(strcmp(tokens[0],"alias")==0 && tokens[1]!=NULL && strcmp(tokens[1], "") != 0 && (tokens[2]==NULL || strcmp(tokens[2], "") == 0)){
        fprintf(stderr, "Alias needs 2 parameters, only 1 was given\n");
        free_tokens(tokens);
        return NULL;
    }

     if(strcmp(tokens[0],"alias")==0 && tokens[1]!=NULL && tokens[2]!=NULL)
    {
        char key_temp[MAX_INPUT_LENGTH];
        memset(key_temp, '\0', MAX_INPUT_LENGTH);
        strcpy(key_temp, tokens[1]);

        add_replace(aliases, key_temp, &tokens[2]);

        free_tokens(tokens);
        return NULL;

    }
     else if(strcmp(tokens[0],"alias")==0 && (tokens[1]==NULL || (strcmp(tokens[1], "") == 0)))
    {
        print_alias(aliases);
        free_tokens(tokens);
        return NULL;
    }
     else if (strcmp(tokens[0],"unalias")==0 && tokens[1]!=NULL && tokens[2]==NULL)
    {
        delete_alias(aliases,tokens[1]);
        free_tokens(tokens);
        return NULL;
    }
     else{
        free_tokens(tokens);
        return NULL;
    }

}


/*
 * Creates or opens .aliases file, saves every line of alias with the corresponding key and closes
 * when there is no more to write.
 */
int save_aliases(AList aliases) {
    Alias* current = *aliases;
    char *filepath = malloc(sizeof(char) * MAX_INPUT_LENGTH);
    memset(filepath, '\0', MAX_INPUT_LENGTH);

    strcpy(filepath, getenv("HOME"));
    strcat(filepath, "/.aliases");
    FILE *file = fopen(filepath, "w+");

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
    return 1;
}


/*
 * Opens .aliases file, reads in every line and adds to alias list.
 */
int load_aliases(AList aliases) {
	char *filepath = malloc(sizeof(char) * MAX_INPUT_LENGTH);
	memset(filepath, '\0', MAX_INPUT_LENGTH);
    strcpy(filepath, getenv("HOME"));
    strcat(filepath, "/.aliases");
    FILE *file = fopen(filepath, "r+");
    
    char line[MAX_INPUT_LENGTH];
    char temp[MAX_INPUT_LENGTH];
    char *key;
    char *value;

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
            value = temp + strlen(key) + 1;
            strcat(value, "\0");

            add_alias(aliases, key, value);
        }
    } // Other wise adds alias grabbed from each line

    fclose(file);
    free(filepath);
    return TRUE;
}

int free_aliases(AList aliases){
    Alias* current = *aliases;
    Alias* prev = NULL;

    if(current==NULL)
    {
        return -1;
    }

    while(current!=NULL){
        prev = current;
        current = current->next;

        free(prev->key);
        free(prev->value);
        free(prev);

    }

    return 1;
}


char* getvalue(char*key,AList aliases)
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
char** substituteAlias(char** tokens,AList aliases)
{
    char temp[MAX_INPUT_LENGTH];
    int index=0;
    while (tokens[index] != NULL)
    {
        if (strcmp(tokens[0],"unalias")==0||strcmp(tokens[0],"alias")==0)
        {
            return tokens;
        }
        if (getvalue(tokens[index],aliases)!=NULL) {
            char copy[MAX_INPUT_LENGTH];

            strcpy(copy, getvalue(tokens[index], aliases));

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

    return tokeniseString(final);
}
