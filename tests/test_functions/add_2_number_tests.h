//
// Created by zkiss on 28/01/2021.
//

#ifndef CS210_SIMPLE_SHELL_ADD_2_NUMBER_TESTS_H
#define CS210_SIMPLE_SHELL_ADD_2_NUMBER_TESTS_H

#endif //CS210_SIMPLE_SHELL_ADD_2_NUMBER_TESTS_H


// Test 1 -- Add 2 positive number -------------
void add_2_positive_number(struct Test *test_list, int *i){
    int temp = *i;
    test_list[temp].testName = malloc(TEST_TITLE_SIZE);

    // TESTING METHODS FROM HERE ------------
    char test_title[] = "Add 2 positive number";

    int result = 13;
    int a = 5;
    int b = 8;

    if(result == add_2_int_number(a,b)){
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

// Test 2 -- Add 2 negative number -------------
void add_2_negative_number(struct Test *test_list, int *i){
    int temp = *i;
    test_list[temp].testName = malloc(TEST_TITLE_SIZE);

    // TESTING METHODS FROM HERE ------------
    char test_title[] = "Add 2 negative number";

    int result = -13;
    int a = -5;
    int b = 8;
    if(result == add_2_int_number(a,b)){
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

void run_add_2_number_tests(struct Test *test_list, int *i){
    add_2_positive_number(test_list, i);
    add_2_negative_number(test_list, i);
}