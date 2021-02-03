//
// Contains methods to display a welcome message, print the command prompt, and read user input.
// Created by Eamonn McClay on 28/01/2021.
//

// Displays a welcome message.
int welcome() {
  printf("CS210 Group 5 Simple Shell\n");
  return 0;
}

// Prints the prompt for the user to enter a command.
int print_prompt() {
    printf("> ");
    return 0;
}

// Reads the users input as a string.
char* read_input() {
    static char input[514];

    if(fgets(input, 514, stdin)==NULL)		//Ctr-D detected
    {
        return NULL;
    }
    else if(strlen(input)==1)		//empty input
    {
        printf("Enter a valid command\n");
        setbuf(stdin,NULL);
        print_prompt();
        return read_input();			//does not actually return anything just calls itself until there is either a valid input or CTRL-d
    }
    else if(input[strlen(input)-1]!='\n')
    {
        printf("Input too long\n");
        //when the input is too long
        while(getchar()!='\n')
        {

        }

        print_prompt();
        return read_input();

    }
    else
        return input;


}
