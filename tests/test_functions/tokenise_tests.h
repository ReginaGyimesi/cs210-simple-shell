//
// Created by Zoltan Kiss on 2021-02-02.
//

#ifndef CS210_SIMPLE_SHELL_TOKENISE_TESTS_H
#define CS210_SIMPLE_SHELL_TOKENISE_TESTS_H

#endif //CS210_SIMPLE_SHELL_TOKENISE_TESTS_H

void tokenise_NULL_input(struct Test *test_list, int *i){
    int temp = *i;
    test_list[temp].testName = malloc(TEST_TITLE_SIZE);

    // TESTING METHODS FROM HERE ------------
    char test_title[] = "Tokenise NULL input";   // Add title

    char** result = NULL;

    if(tokenise(NULL) == result){
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

void tokenise_empty_input(struct Test *test_list, int *i){
    int temp = *i;
    test_list[temp].testName = malloc(TEST_TITLE_SIZE);

    // TESTING METHODS FROM HERE ------------
    char test_title[] = "Tokenise empty input";  // Add title

    char** result = malloc(sizeof(char*));
    result[0] = "\0";

    char input[80] = "";

    char** tokenized = tokenise(&input[0]);

    if(tokenized == NULL){
        test_list[temp].testPassed = TRUE;
    }
    else{
        test_list[temp].testPassed = FALSE;
    }

    result = NULL;


    // TESTING METHODS STOPS HERE ---------
    strcpy(test_list[temp].testName, test_title);
    if(temp < NUMBER_OF_TESTS){
        (*i)++;
    }

}

void tokenise_complex_input(struct Test *test_list, int *i){
    int temp = *i;
    test_list[temp].testName = malloc(TEST_TITLE_SIZE);

    // TESTING METHODS FROM HERE ------------
    char test_title[] = "Tokenise complex input";  // Add title

    char** result = malloc(sizeof(char*)*8);
    result[0] = "ls";
    result[1] = "-lF";
    result[2] = ".";
    result[3] = "..";
    result[4] = ".";
    result[5] = "..";
    result[6] = "/";
    result[7] = "fksdk";

    char input[80] = "ls\t-lF;.&..>.<..|/\tfksdk";

    char** tokenized = tokenise(&input[0]);

    for(int j = 0; j < 8; ++j){
        if(strcmp(tokenized[j], result[j]) != 0){
            test_list[temp].testPassed = FALSE;
            break;
        }
        else{
            test_list[temp].testPassed = TRUE;
        }
    }

    result = NULL;


    // TESTING METHODS STOPS HERE ---------
    strcpy(test_list[temp].testName, test_title);
    if(temp < NUMBER_OF_TESTS){
        (*i)++;
    }

}

void run_tokenise_tests(struct Test *test_list, int *i){
    tokenise_NULL_input(test_list, i);
    tokenise_empty_input(test_list, i);
    tokenise_complex_input(test_list, i);
}