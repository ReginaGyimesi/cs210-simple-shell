//
// Created by Zoltan Kiss on 2021-02-02.
//

#ifndef CS210_SIMPLE_SHELL_APPLYCOMMAND_TESTS_H
#define CS210_SIMPLE_SHELL_APPLYCOMMAND_TESTS_H

#endif //CS210_SIMPLE_SHELL_APPLYCOMMAND_TESTS_H

void apply_NULL_input(struct Test *test_list, int *i){
    int temp = *i;
    test_list[temp].testName = malloc(TEST_TITLE_SIZE);

    // TESTING METHODS FROM HERE ------------
    char test_title[] = "Apply command on NULL input";   // Add title

    int result = -1;
    if(apply_command(NULL) == result){
        test_list[temp].testPassed = TRUE;
    }
    else{
        test_list[temp].testPassed = FALSE;
    }


    // TESTING METHODS STOPS HERE ---------
    strcpy(test_list[temp].testName, test_title);
    if(temp < NUMBER_OF_TESTS){
        (*i)++;
    }

}

void apply_NULL_input_args0(struct Test *test_list, int *i){
    int temp = *i;
    test_list[temp].testName = malloc(TEST_TITLE_SIZE);

    // TESTING METHODS FROM HERE ------------
    char test_title[] = "Apply command on args[0] NULL";  // Add title

    int result = -1;
    char **tokens = {NULL};

    if(apply_command(tokens) == result){
        test_list[temp].testPassed = TRUE;
    }
    else{
        test_list[temp].testPassed = FALSE;
    }


    // TESTING METHODS STOPS HERE ---------
    strcpy(test_list[temp].testName, test_title);
    if(temp < NUMBER_OF_TESTS){
        (*i)++;
    }

}

void apply_exit_input(struct Test *test_list, int *i){
    int temp = *i;
    test_list[temp].testName = malloc(TEST_TITLE_SIZE);

    // TESTING METHODS FROM HERE ------------
    char test_title[] = "Apply command on exit";  // Add title

    int result = 0;
    char **tokens = malloc(sizeof(char*));
    tokens[0] = "exit";

    if(apply_command(tokens) == result){
        test_list[temp].testPassed = TRUE;
    }
    else{
        test_list[temp].testPassed = FALSE;
    }

    tokens = NULL;

    // TESTING METHODS STOPS HERE ---------
    strcpy(test_list[temp].testName, test_title);
    if(temp < NUMBER_OF_TESTS){
        (*i)++;
    }

}

void apply_garbage_input(struct Test *test_list, int *i){
    int temp = *i;
    test_list[temp].testName = malloc(TEST_TITLE_SIZE);

    // TESTING METHODS FROM HERE ------------
    char test_title[] = "Apply command on garbage";  // Add title

    int result = 1;
    char **tokens = malloc(sizeof(char*));
    tokens[0] = "asdwegfegvsdv";

    if(apply_command(tokens) == result){
        test_list[temp].testPassed = TRUE;
    }
    else{
        test_list[temp].testPassed = FALSE;
    }

    tokens = NULL;


    // TESTING METHODS STOPS HERE ---------
    strcpy(test_list[temp].testName, test_title);
    if(temp < NUMBER_OF_TESTS){
        (*i)++;
    }

}

void apply_multiple_tokens_input(struct Test *test_list, int *i){
    int temp = *i;
    test_list[temp].testName = malloc(TEST_TITLE_SIZE);

    // TESTING METHODS FROM HERE ------------
    char test_title[] = "Apply command on multiple tokens";  // Add title

    int result = 1;
    char **tokens = malloc(sizeof(char*) + 3);
    tokens[0] = "This";
    tokens[1] = "is";
    tokens[2] = "exit";

    if(apply_command(tokens) == result){
        test_list[temp].testPassed = TRUE;
    }
    else{
        test_list[temp].testPassed = FALSE;
    }

    // TESTING METHODS STOPS HERE ---------
    strcpy(test_list[temp].testName, test_title);
    if(temp < NUMBER_OF_TESTS){
        (*i)++;
    }

}

void run_applycommand_tests(struct Test *test_list, int *i){
    apply_NULL_input(test_list, i);
    apply_NULL_input_args0(test_list, i);
    apply_exit_input(test_list, i);
    apply_garbage_input(test_list, i);
    apply_multiple_tokens_input(test_list, i);
}