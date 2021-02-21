//
// Contains methods for handling aliases,
// including methods to add, remove, print, and invoke aliases.
// Created by Eamonn McClay 20/02/2021
//

#ifndef CS210_SIMPLE_SHELL_ALIAS_HANDLER_H
#define CS210_SIMPLE_SHELL_ALIAS_HANDLER_H

#endif //CS210_SIMPLE_SHELL_ALIAS_HANDLER_H

struct Alias {
	char *key;
	char *command;
	struct Alias *next;
};

struct Alias *head = NULL;

int alias_length() {
	int length = 0;
	
	struct Alias *ptr = head;

	while(ptr != NULL) {
		ptr = ptr->next;
		length++;
	}

	return length;
}

struct Alias *alias_search(char* key) {
	struct Alias *ptr = head;

	if(head == NULL) {
		return NULL;
	}
	else {
		while(ptr->key != key) {
			if(ptr->next == NULL) {
				return NULL;
			}
			else {
				ptr = ptr->next;
			}
		}
		
		return ptr;
	}	
}

int add_alias(char* key, char* command) {
	if(alias_length() > 20) {
		fprintf(stderr, "Maximum amount of 20 stored aliases exceeded. Please delete or overwrite an alias to continue\n");
        return ERROR;
	}

	struct Alias *search = alias_search(key);

	if(search != NULL) {
		search->command = command;
		return TRUE;
	}

	struct Alias *new = (struct Alias*)malloc(sizeof(struct Alias));
	new->key = key;
	new->command = command;
	new->next = head;
	head = new;
	return TRUE;
}

/*
 * Removes specific alias from Alias list, error if no key is entered,
 * otherwise returns true if removal is successful.
 */
int remove_alias(char* key) {
    struct Alias* temp = head;
    struct Alias* prev = NULL;

    if(key == NULL)
        printf("Enter valid key."); // error if key is empty
        return ERROR;

    if (temp != NULL && temp->command == key)
    {
        head = temp->next; // changed head
        free(temp);        // free old head
        return TRUE;
    }
    else
    {
        while (temp != NULL && temp->command != key)
        {
            prev = temp;
            temp = temp->next;
        }

        if (temp == NULL)
            return TRUE;

        prev->next = temp->next;
        free(temp);
    }
}

/*
 * Prints the list of aliases that were set. If head is NULL returns error,
 * otherwise prints them.
 */
int print_alias() {
    struct Alias *p = head;
    if(head == NULL) // if head is NULL then there are no aliases set
        printf("No aliases set.");
        return ERROR;

    while (head != NULL) {
        printf(" %d ", head->command);
        head = head->next;
    }
}