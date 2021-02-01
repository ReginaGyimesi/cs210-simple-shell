// 
// Function to take in an input string and split it into separate tokens.
// Returns a pointer to a null-terminated list of char pointers.
// Created by Mark Oppo on 29/01/2021.
//
#include <stdlib.h>

#define DELIMITER_SIZE 10

char** tokenise(char* input) {

    if(input == NULL){
        return NULL;
    }
	
	char delimiters[DELIMITER_SIZE] = {' ', '\t', '|', '>', '<', '&', ';', '\n', '-', '/'};
	int n_token = 1;
	char* current = input;
	char new_line[512] = {'\0'};

	int flag = 0;
	while (*current != '\0') {
		for (int i = 0; i < DELIMITER_SIZE; i++) {
			if (*current == delimiters[i]) {
			    if(*current == '-'){
			        new_line[flag++] = ' ';
			        new_line[flag++] = *current;
                    n_token ++;
                    *current = ' ';
                    break;
			    }
			    else if(*current == '/'){
                    new_line[flag++] = ' ';
                    new_line[flag++] = *current;
                    new_line[flag++] = ' ';
                    n_token ++;
                    *current = ' ';
                    break;
			    }
			    else{
                    n_token ++;
                    *current = ' ';
                    new_line[flag++] = *current;
                    break;
			    }
			}
		}
        if(*current != ' '){
            new_line[flag++] = *current;
        }
		current ++;

	}
	
	char** tokens = malloc(sizeof(char*) * (n_token + 1));
	current = &new_line[0];
	n_token = 0;
	int prev_is_delim = 1;
	
	while (*current != '\0') {
		if (*current == ' ') {
			prev_is_delim = 1;
			*current = '\0';
		}
		else if (prev_is_delim == 1) {
			prev_is_delim = 0;
			tokens[n_token] = current;
			n_token ++;
		}
		current ++;
	}

	tokens[n_token] = NULL;
	return tokens;
}