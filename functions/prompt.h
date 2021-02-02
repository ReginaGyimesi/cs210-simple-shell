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
  static char input[517];
  fgets(input, 517, stdin);

  if(strlen(input) == 516) {
      printf("Input exceeds limit of 512 characters.\n");
      setbuf(stdin, NULL);
      return NULL;
  }
  else if (strlen(input) == 1) {
      printf("Please enter a command.\n");
      return NULL;
  }
  else {
    return input;
  }
}
