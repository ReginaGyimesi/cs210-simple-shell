//
// Created by bilko26 on 2021. 02. 26..
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



Alias *new_alias(char* key,char*value){
    Alias *temp = (Alias*) malloc(sizeof(Alias));
    temp->key = malloc(sizeof (char) * strlen(key));
    temp->value = malloc(sizeof (char) * strlen(value));

    strcpy(temp->key, key);
    strcpy(temp->value, value);
    temp->next = NULL;
    return temp;
}


void add_alias(AList l,char*key,char*value)
{
  //  char key1=*key;
    //char value1=*value;

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
        printf("%s %s \n",curr->key,curr->value);
        curr=curr->next;
    }

}



int replace_if_exists(AList l, char* key, char* value)
{

    Alias *current=*l;
    while(current!=NULL)
    {
        if(strcmp(current->key,key)==0)
        {
            free(current->value);
            current->value = malloc(sizeof (char) * strlen(value));
            strcpy(current->value, value);
            return 1;
        }
        else
        {
            current=current->next;
        }

    }
    return -1;

}

int add_replace(AList l,char*key,char*value)
{
    int status=replace_if_exists(l,key,value);
    if(status==-1)
    {
        add_alias(l,key,value);
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
            previous->next=NULL;

            free(curr->key);
            free(curr->value);
            free(curr);
            return 1;


        }



    }
    printf("Alias with these parameters was not found\n");
    return -1;
}


int check_alias(char** tokens, AList aliases) {

     if(strcmp(tokens[0],"alias")==0 && tokens[1]!=NULL && tokens[2]!=NULL && tokens[3]==NULL)
    {
        add_replace(aliases,tokens[1],tokens[2]);
        printf("create an alias\n");
        return 1;
    }if(strcmp(tokens[0],"alias")==0&&tokens[1]==NULL)
    {
        print_alias(aliases);
        return 1;
    }

    else if (strcmp(tokens[0],"unalias")==0&&tokens[1]!=NULL&&tokens[2]==NULL)
    {
        return delete_alias(aliases,tokens[1]);
    }
    else
        return -1;
}