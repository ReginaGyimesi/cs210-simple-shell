//
// Created by bilko26 on 2021. 02. 26..
//

#ifndef CS210_SIMPLE_SHELL_ALIAS_HANDLER_CLEAN_H
#define CS210_SIMPLE_SHELL_ALIAS_HANDLER_CLEAN_H

#endif //CS210_SIMPLE_SHELL_ALIAS_HANDLER_CLEAN_H
struct Alias {
    char *key;
    char *command;
    struct Alias *next;
}; // Alias struct defined
int alias_length_clean(struct Alias **head) {
    int length = 0;

    struct Alias *ptr = *head; // Pointer initialised

    while(ptr != NULL) {
        ptr = ptr->next;
        length++;
    } // Iterate and count through list using pointer

    return length;
}
struct Alias *search(struct Alias **head,char* key) {
    struct Alias *ptr = *head; // Pointer initialised at head of list

    if(ptr == NULL) {
        return NULL;
    } // Return NULL if list empty
    else {
        while(ptr->key != key) {
            if(ptr->next == NULL) {
                return NULL;
            } // Key not found in list
            else {
                ptr = ptr->next;
            } // Iterate through list using pointer
        }

        return ptr; // Return alias if key found, otherwise return NULL
    }


    int add_alias_clean(struct Alias **head,char *key,char* value)
    {
    struct Alias *ptr = search(head,key); // Pointer initialised at head of list
        if(ptr==NULL)//we did not find the key in the linkedList
        {
            ptr = (struct Alias*)malloc(sizeof(struct Alias)); //creating a new Alias
            ptr->key=key;
            ptr->command=value;
            ptr->next=*head;
            head=&ptr;      //adding it to the beginning of the list
            return TRUE;

        }
        else        //else updating the value of the key
        {
            ptr->command=value;
            return TRUE;
        }
        //else something went wrong
        return ERROR;

    }

    int remove_alias_clean(struct Alias **head, char* key) {
        struct Alias *temp = *head; // Stores head
        struct Alias *prev = NULL;

        if(key == NULL) {
            fprintf(stderr, "Invalid key.\n"); // Error if key is empty
            return ERROR;
        }
        if (temp != NULL && temp->command == key) // If head is to be removed
        {
            *head = temp->next; // Changes head
            free(temp);        // Frees old head
            return TRUE;
        }
        else
        {
            while (temp != NULL && temp->command != key) // Searches for the command given by the key
            {
                prev = temp;
                temp = temp->next;
            }

            if (temp == NULL ) {
                fprintf(stderr, "Key not found.\n");
                return ERROR;
            }
            prev->next = temp->next;  // Otherwise remove alias
            free(temp);
            return TRUE;
        }
    }
    int print_alias(struct Alias **head) {
        struct Alias *ptr = *head;
        if(ptr == NULL) // If head is NULL then there are no aliases set.
            fprintf(stderr, "No aliases set.\n");

        while (ptr != NULL) {
            printf(" %s ", ptr->command);
            ptr = ptr->next;
        }
    }
    int check_alias(char** tokens,struct Alias** head) {

        if (strcmp(tokens[0], "alias") == 0 && tokens[1] == NULL){
            return print_alias(head);
        }
    }





}