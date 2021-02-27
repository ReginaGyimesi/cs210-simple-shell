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







Alias *new_alias(char* key,char*value){
    Alias *temp = (Alias*) malloc(sizeof(Alias));
    temp->key= key;
    temp->value=value;
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
    while(curr!=NULL)
    {
        printf("%s, %s",curr->key,curr->value);
        curr=curr->next;
    }

}



int replace_if_exists(AList l ,char* key,char* value)
{

    Alias *current=*l;
    while(current!=NULL)
    {
        if(current->key==key)
        {
            current->value=value;
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
    if(status>-1)
    {
        add_alias(l,key,value);
        return 1;
    }
    return status;
}

int delete_alias(AList l,char*key)
{

    Alias* curr=*l;
    Alias* previous=NULL;
    if(curr==NULL)
    {
        printf("No aliases have been set\n"); //no aliases have been set invalid operation
        return -1;
    }
    else if(curr->key==key) //if first element is what we're looking for
    {
        if(curr->next!=NULL)//if its not the only element list head points to next element
        {
            l=&curr->next;
        }
        else{
            l=NULL; //if it is then we set the list to NULL
        }
        free(curr);
        return 1;
    }
    while(curr->next!=NULL) //iterate to the end of the list
    {
        previous=curr;
        curr=curr->next;

        if(curr->key=key)
        {
            if(curr->next!=NULL)
            {
                previous->next=curr->next;
                free(curr);
                return 1;
            }
            previous->next=NULL;
            free(curr);
            return 1;


        }

        curr=curr->next;



    }
    printf("Alias with these parameters was not found\n");
    return -1;
}


}