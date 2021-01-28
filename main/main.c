//
// Created by zkiss on 27/01/2021.
//

#include <stdio.h>
#include <main.h>

int main(int argc, char *argv[]){

    testing(TRUE);     //Set it to TRUE if you want to run every test

    int a;
    int b;

    printf("Give 1. number: ");
    scanf("%d", &a);

    printf("Give 2. number: ");
    scanf("%d", &b);

    int result = add_2_int_number(a, b);

    printf("Result is %d", result);

    return 0;
}

void testing(int run_every_test){
    if(run_every_test){
        run_tests();
    }

}