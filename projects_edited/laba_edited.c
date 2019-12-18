/*
	Created by Callgrind ฅ^•ﻌ•^ฅ
	Thu Dec 19 00:35:23 2019
*/
#include "../headers/analyze.h"
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include <math.h>

void swap(double * a, double * b)
{
	add_sensor("swap()", callgrind_checker++);
	double c;
	c = *a;
	*a = *b;
	*b = c;
	callgrind_checker--;
}



void f1(double *mass, int n)//?????????? ???.
{
	add_sensor("f1()", callgrind_checker++);
	int i,j;
	for (i = 0; i < n - 1; i++)
		for (j = 0; j < n - i - 1; j++)
			if (mass[j] > mass[j + 1])
				swap(&mass[j],&mass[j + 1]);
	callgrind_checker--;
}



void f2(double *mass, int n)//?????????? ????????
{
	add_sensor("f2()", callgrind_checker++);
	double bufer;
	if (n==2)
	{
		if (mass[0] > mass[1])
			swap(&mass[0], &mass[1]);
	{callgrind_checker--;
		return;
	}
	}
	if (n == 1)
	{callgrind_checker--;
		return;
	}
	if (n>2)
	{
		int pointer1;
		int pointer2;
		int len1;
		int len2;
		pointer1 = 0;
		pointer2 = n - n / 2;
		len1 = n -  n / 2;
		len2 = n / 2;
		f2(mass + pointer1, len1);
		f2(mass + pointer2, len2);
		int i,j;
		for (i = 0;len1!=0 && len2!=0;)
		{
			if (mass[pointer1] > mass[pointer2])
			{
				bufer = mass[pointer2];
				for (j = pointer2; j > pointer1; j--)
					mass[j] = mass[j - 1];
				len2--;
				mass[pointer1++] = bufer;
				pointer2++;
			}
			else
			{
				pointer1++;
				len1--;
			}
		}//for
	}//if
	{callgrind_checker--;
	return;
	}
}//f2




int BinarySearch(double *arr, int n, int x)//???????? ?????
{
	add_sensor("BinarySearch()", callgrind_checker++);
	static int stepen = 1;
	if (n > 0)n--;
	{callgrind_checker--;
	if ((arr[0] != x) && (n == 0)) { stepen = 0; return -1; }
	}
	if (arr[n / 2] == x) {
	    stepen = 1;
	{callgrind_checker--;
	    return n / 2 + 1;
	}
	}
	if (arr[n / 2] < x) {
	    stepen = 1;
	{callgrind_checker--;
	    return pow(n / 2 + 1 + BinarySearch(arr + n / 2 + 1, n - n / 2, x) + 2, stepen) - 2;
	}
	}
	if (arr[n / 2] > x) {
	    stepen = 1;
	{callgrind_checker--;
	    return pow(BinarySearch(arr, n / 2, x) + 2, stepen) - 2;
	}
	}
}
struct diapozon
{
	int begin;
	int end;
};
struct diapozon f3(double* mass,int n, double x)// ???. ?????
{
	add_sensor("f3()", callgrind_checker++);
	struct diapozon ret;
	ret.begin = ret.end = BinarySearch(mass, n, x);
	if (ret.begin==-1)
	{callgrind_checker--;
		return ret;
	}
	short forBegin = 1, forEnd = 1;
	while ((ret.begin!=1 || ret.end!=n)&& forBegin && forEnd)
	{
		if (ret.begin != 1 && forBegin)
			if (mass[ret.begin - 2] == x)
				ret.begin--;
			else
				forBegin = 0;
		if (ret.end != n && forEnd)
			if (mass[ret.end] == x)
				ret.end++;
			else
				forEnd = 0;
	} 
	{callgrind_checker--;
	return ret;
	}
}

int main()
{
	add_sensor("main()", callgrind_checker++);
	int i;
	printf("VVedi razmer masiva:  ");
	int n = 0;
	scanf("%i", &n);
	double* mass=(double*)malloc(sizeof(double)*2 * n);
	system("cls");
	printf("Zapolnit masiv \n\t1) s klaviaturu\n\t2) random\n");
	int vvod;
	do {
		scanf("%i", &vvod);	
		system("cls");
	
		switch (vvod)
		{
		case 1:
			for ( i = 0; i < 2 * n; i++)
			{			
				printf("VVedite chislo [%i]: ",i);
				scanf("%lf", &mass[i]);
			}
			system("cls");
			break;
		case 2:
			for ( i = 0; i < 2 * n; i++)
				mass[i] = rand()%(2 * n);
			break;
		default:

			printf("Vvuberite punkt menu:  ");
			break;
		}
	}
	while (vvod!=1&& vvod != 2);
	printf("VVedi chislo x:  ");
	double x;
	scanf("%lf", &x);
	system("cls");
	printf("\n massiv otsortirovan:  ");
	f1(mass, 2 * n);
	for ( i = 0; i < 2 * n; i++)
		printf("%lf ", mass[i]);
    struct	diapozon ret = f3(mass,2*n,x);
	if (ret.begin == -1)
		printf("\n chisla x v masive net ");
	else if(ret.begin==ret.end)
		printf("\n chislo x v masive: %i", ret.begin);
	else
	printf("\n chislo x v masive: %i - %i ",ret.begin,ret.end);
	free(mass);
	{callgrind_checker--;
	return 1;
	}
}
