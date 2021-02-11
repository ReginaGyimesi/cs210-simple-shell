//
// Contains methods for handling the command history,
// including methods to store, print and execute previous commands.
// Created by Mark Oppo, Regina Gyimesi and (add other names) 10/02/2021
//

void make_history(char** dest) {
    
    for (int k = 0; k < 20; k++) {
        dest[k] = malloc(sizeof(char) * 514);
        dest[k][0] = '\0';
    }
}

void free_history(char** dest) {
    for (int k = 0; k < 20; k++) {
        free(dest[k]);
    }

    dest = NULL;
}

void add_to_history(char* input, char* history[]) {

    int i = 0;
    while (i < 20 && *(history[i]) != '\0') {
        i++;
    }

    if (i == 20) {    // array is full
        char *temp = history[0];
        for (int j = 1; j <= 19; j++) {
            history[j - 1] = history[j];
        }
        history[19] = temp;
        strcpy(history[19], input);
    } else {
        strcpy(history[i], input);
    }
}

int exec_number_history(int number, char* history[]) {
        for (int i = 1; i <= 20 && *(history[i]) != '\0'; i++) {
            if (i == number) {
                printf("History command: %s", history[number]);
                return TRUE;
            }
        }
        perror("History cannot be executed");
        return FALSE;
    }

int exec_minus_number_history(int number, char* history[]) {
    for (int i = 1; i <= 20 && *(history[i]) != '\0'; i++) {
        if (i == number) {
            int current = i  - number;
            if(current > 0) {
            printf("History command: %s", history[current]);
            return TRUE;
            }
        }
    }
    perror("History cannot be executed");
    return FALSE;
}
