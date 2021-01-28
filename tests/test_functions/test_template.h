//
// Created by zkiss on 28/01/2021.
//

#ifndef CS210_SIMPLE_SHELL_TEST_TEMPLATE_H
#define CS210_SIMPLE_SHELL_TEST_TEMPLATE_H

#endif //CS210_SIMPLE_SHELL_TEST_TEMPLATE_H

/* This is a template test file
 * You can copy this and add new tests for the code
 * Please only modify what is between the commented lines for each method
 * Of course you can add more test methods
 */


void test_1(struct Test *test_list, int *i){
    int temp = *i;
    test_list[temp].testName = malloc(TEST_TITLE_SIZE);

    // TESTING METHODS FROM HERE ------------
    char test_title[] = "";   // Add title


    test_list[temp].testPassed = TRUE;  //Make sure to set the test result in the end

    // TESTING METHODS STOPS HERE ---------
    strcpy(test_list[temp].testName, test_title);
    if(temp < NUMBER_OF_TESTS){
        (*i)++;
    }

}

void test_2(struct Test *test_list, int *i){
    int temp = *i;
    test_list[temp].testName = malloc(TEST_TITLE_SIZE);

    // TESTING METHODS FROM HERE ------------
    char test_title[] = "";  // Add title


    test_list[temp].testPassed = TRUE;  //Make sure to set the test result in the end


    // TESTING METHODS STOPS HERE ---------
    strcpy(test_list[temp].testName, test_title);
    if(temp < NUMBER_OF_TESTS){
        (*i)++;
    }

}

void run_template_tests(struct Test *test_list, int *i){
    test_1(test_list, i);
    test_2(test_list, i);
}