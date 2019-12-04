//
// Created by delvin on 23.11.2019.
//
#include <stdio.h>
#include <stdlib.h>

int sum(int a, int b){
    return a+b;
}

int main()
{
    float sum_res;
    printf("%s", "Hello world\n");
    sum_res = (float)sum(2,4);
    printf("%f\n", sum_res);
    return 0;
}
