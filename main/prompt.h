//
// Contains methods to display a welcome message, print the command prompt, and read user input.
// Created by Eamonn McClay on 28/01/2021.
//

#define MAX_INPUT_LENGTH 514

// Displays a welcome message.
int welcome() {
    printf("CS210 Group 5 Simple Shell %s\n", VERSION);
    return 0;
}

// Prints the prompt for the user to enter a command.
int print_prompt() {
    printf("> ");
    return 0;
}

// Reads the users input as a string.
char* read_input() {
    //static char input[514];
    char* input = malloc(sizeof(char)*MAX_INPUT_LENGTH);
    fgets(input, MAX_INPUT_LENGTH, stdin);
    if(input==NULL || *input == '\0')		//Ctr-D detected
    {
        return NULL;
    }
    else if(strlen(input)==1)		//empty input
    {
        printf("Enter a valid command\n");
         fflush(stdin);
        return input;			//does not actually return anything just calls itself until there is either a valid input or CTRL-d
    }
    else if(input[strlen(input)-1]!='\n' && strlen(input)<MAX_INPUT_LENGTH-1){
        return NULL;
    }
    else if(input[strlen(input)-1]!='\n' && strlen(input)>=MAX_INPUT_LENGTH-1)
    {
        printf("Input too long\n");
        fflush(stdin);
        memset(input, '\0', sizeof(char)*514);
        input[0] = '\n';
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
        return input;
    }

    return input;

}
