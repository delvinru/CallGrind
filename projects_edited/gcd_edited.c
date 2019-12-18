/*
	Created by Callgrind ฅ^•ﻌ•^ฅ
	Thu Dec 19 00:35:23 2019
*/
#include "../headers/analyze.h"
#include <stdio.h>

int getGCD(int a, int b)
{
	add_sensor("getGCD()", callgrind_checker++);
	if(b == 0)
	{callgrind_checker--;
		return a;
	}
	a = a >= 0 ? a : -a;
	b = b >= 0 ? b : -b;
	if(a>b)
	{callgrind_checker--;
		return getGCD(b, a%b);
	}
	else 
	{callgrind_checker--;
		return getGCD(a, b%a);
	}
}

int main()
{
	add_sensor("main()", callgrind_checker++);
	int a, b;
	printf("Введите a: "); scanf("%d", &a);
	printf("Введите b: "); scanf("%d", &b);
	printf("GCD: %d\n", getGCD(a, b));
	{callgrind_checker--;
	return 0;
	}
}