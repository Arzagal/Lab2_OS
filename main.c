#include <stdio.h>
#include "header.h"

int main() {
    // The main function is used to test our function
    int* test1 = malloc_is(10* sizeof(int));
    int* test2 = malloc_is(10* sizeof(int));
    test2[0] = 45;
    test1[0] = 0;
    test1[2] = 6;
    test1[9] = 9;
    test1[5] = 69;
    printf("test10 : %d, test1_2: %d, test1_9 : %d, test2_0 : %d\n", test1[0], test1[2], test1[9], test2[0]);
    free_is(test1);
    int* test3 = malloc_is(10* sizeof(int));
    free_is(test2);
    int* test4 = malloc_is(2* sizeof(int));
    int* test5 = malloc_is(2* sizeof(int)); //
    // At this point we used the debug to check the memory and the value of the pointers
    return 0;
}
