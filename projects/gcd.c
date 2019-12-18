#include <stdio.h>

int getGCD(int a, int b)
{
	if(b == 0)
		return a;
	a = a >= 0 ? a : -a;
	b = b >= 0 ? b : -b;
	if(a>b)
		return getGCD(b, a%b);
	else 
		return getGCD(a, b%a);
}

int main()
{
	int a, b;
	printf("Введите a: "); scanf("%d", &a);
	printf("Введите b: "); scanf("%d", &b);
	printf("GCD: %d\n", getGCD(a, b));
	return 0;
}