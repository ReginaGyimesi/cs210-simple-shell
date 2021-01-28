//
// Created by zkiss on 27/01/2021.
//

#ifndef CS210_SIMPLE_SHELL_TESTS_H
#define CS210_SIMPLE_SHELL_TESTS_H

#endif //CS210_SIMPLE_SHELL_TESTS_H

#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "test_functions/test_init.h"

// ADD INCLUDES FROM HERE
#include "test_functions/add_2_number_tests.h"

void check_tests();

int run_tests(){
    int i = 0;
    test_list = malloc(NUMBER_OF_TESTS * sizeof(struct Test));

    // ADD DRIVER FUNCTIONS FROM HERE
    run_add_2_number_tests(test_list, &i);


    // STOP HERE pls
    check_tests();

    return 0;
}

void check_tests(){
    for(int i = 0; i < NUMBER_OF_TESTS; ++i){
        printf("#%d %s Passed: %s", i, test_list[i].testName, test_list[i].testPassed ? "TRUE\n" : "FALSE\n");
    }
}