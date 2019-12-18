/*
	Created by Callgrind ฅ^•ﻌ•^ฅ
	Thu Dec 19 00:35:23 2019
*/
#include "../headers/analyze.h"
//
// Created by delvin on 23.11.2019.
//
#include <stdio.h>
#include <stdlib.h>

int sum(int a, int b){
	add_sensor("sum()", callgrind_checker++);
	callgrind_checker--;
    return a+b;
}
int main()
{
	add_sensor("main()", callgrind_checker++);
    float sum_res;
    printf("%s", "Hello world\n");
    sum_res = (float)sum(2,4);
    printf("%f\n", sum_res);
	{callgrind_checker--;
    return 0;
	}
}
