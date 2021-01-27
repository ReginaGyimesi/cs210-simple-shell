//
// Created by zkiss on 27/01/2021.
//

#ifndef CS210_SIMPLE_SHELL_TESTS_H
#define CS210_SIMPLE_SHELL_TESTS_H

#endif //CS210_SIMPLE_SHELL_TESTS_H

//
// Created by zkiss on 27/01/2021.
//

#include <stdlib.h>
#include <stdio.h>
#include "string.h"

#define NUMBER_OF_TESTS 2       //Increase this if you add new tests
#define TEST_TITLE_SIZE 100
#define FALSE 0
#define TRUE 1

struct Test{
    int testPassed;
    char *testName;
};

struct Test test_list[NUMBER_OF_TESTS];

void check_tests();

int run_tests(){
    int i = 0;

    // Test 1 -- Add 2 positive number -------------
    {
        test_list[i].testName = malloc(TEST_TITLE_SIZE);
        strcpy(test_list[i].testName, "Add 2 positive number");

        int result = 13;
        int a = 5;
        int b = 8;
        if(result == add_2_int_number(a,b)){
            test_list[i].testPassed = TRUE;
        }
        else{
            test_list[i].testPassed = FALSE;
        }

        i++;
    }

    // Test 2 -- Add 2 negative number -------------
    {
        test_list[i].testName = malloc(TEST_TITLE_SIZE);
        strcpy(test_list[i].testName, "Add 2 negative number");

        int result = -3;
        int a = 5;
        int b = -8;
        if(result == add_2_int_number(a,b)){
            test_list[i].testPassed = TRUE;
        }
        else{
            test_list[i].testPassed = FALSE;
        }

        i++;
    }


    check_tests();

    return 0;
}

void check_tests(){
    for(int i = 0; i < NUMBER_OF_TESTS; ++i){
        printf("#%d %s Passed: %s", i, test_list[i].testName, test_list[i].testPassed ? "TRUE\n" : "FALSE\n");
    }
}