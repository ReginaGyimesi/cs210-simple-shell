// 
// Function to take in an input string and split it into separate tokens.
// Returns a pointer to a null-terminated list of char pointers.
// Created by Mark Oppo on 29/01/2021.
//
#include <stdlib.h>

char** tokenise(char* input) {
	
	char delimiters[8] = {' ', '\t', '|', '>', '<', '&', ';', '\n'};
	int n_token = 1;
	char* current = input;
	
	while (*current != '\0') {
		for (int i = 0; i < 8; i++) {
			if (*current == delimiters[i]) {
				n_token ++;
				*current = ' ';
				break;
			}
		}
		current ++;
	}
	
	char** tokens = malloc(sizeof(char*) * (n_token + 1));
	current = input;
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