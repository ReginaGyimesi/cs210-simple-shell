//
// Contains methods for handling the command history,
// including methods to store, print and execute previous commands.
// Created by Mark Oppo and (add other names) 10/02/2021
//

void add_to_history(char* input, char* history[]) {
    
    int i = 0;
    while (i < 20 && history[i] != NULL) {
        i ++;
    }
    
    if (i == 20) {    // array is full
        history[0] = NULL;
        for (int j = 1; j <= 19; j++) {
            history[j - 1] = history[j];
        }
        strcpy(history[19], input);
    }
    else {
        strcpy(history[i], input);
    }
}