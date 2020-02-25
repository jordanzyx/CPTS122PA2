
/*******************************************************************************************
* Programmer: Jordan Muehlbauer *
* Class: CptS 122, Spring, 2019;
* Programming Assignment: PA3 *
* Date: Febuary 10th, 2020 *
* Description: This program is a prototype movie player. *
*******************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "source.h"
#include "tests.h"
int main() {
    srand(time(NULL));


    int test = testInsert();
    int test2 = testDelete();
    int test3 = testShuffle();

    int testResults = test + test2 + test3;

    printf("All tests passed = %s\n", testResults == 3 ? "True" : "False" );

    start();
    return 0;
}