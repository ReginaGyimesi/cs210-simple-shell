//
// Created by zkiss on 28/01/2021.
//

#ifndef CS210_SIMPLE_SHELL_TEST_INIT_H
#define CS210_SIMPLE_SHELL_TEST_INIT_H

#endif //CS210_SIMPLE_SHELL_TEST_INIT_H

#define NUMBER_OF_TESTS 10      //Increase this if you add new tests
#define TEST_TITLE_SIZE 100
#define FALSE 0
#define TRUE 1

struct Test{
    int testPassed;
    char *testName;
};

struct Test *test_list;

void check_tests();
void free_tests();