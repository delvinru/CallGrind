/*
	Created by Callgrind ฅ^•ﻌ•^ฅ
	Thu Dec 19 00:35:23 2019
*/
#include "../headers/analyze.h"
int some_function(int a, int b)
{
	add_sensor("some_function()", callgrind_checker++);
	callgrind_checker--;
    return a-b;
}

/*
    ККСО-01-19
    Колесников Алексей

    когда я компилировал стандартом c89 выдавало ошибки, но они не связаны с объявлением массива
    а переписывать мне лень под старый стандарт ¯\_(ツ)_/¯
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#if defined(__linux__)
    #define linux 1
#endif

#define FACTOR 2


/* Help function */

/*
int buble_sort(int *arr, int n)
{
    for(int i = 0; i < n-1; i++)
    {
        for(int j = 0; j < n-i-1; j++)
        {
            if(arr[j] > arr[j+1])
            {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}
*/

double just_double_func(int a, double b)
{
	add_sensor("just_double_func()", callgrind_checker++);
	callgrind_checker--;
    return a+b;
}

int just_int_func(int a, int b)
{
	add_sensor("just_int_func()", callgrind_checker++);
	callgrind_checker--;
    return a - b;
}

float just_float_func(float a, float b)
{
	add_sensor("just_float_func()", callgrind_checker++);
	callgrind_checker--;
    return a*b/a;
}

int field_arr(int *arr, int n)
{
	add_sensor("field_arr()", callgrind_checker++);
    for(int i = 0; i < n; i++)
        arr[i] = rand() % (n+1);
	{callgrind_checker--;
    return 0;
	}
}

int print(int *arr, int n)
{
	add_sensor("print()", callgrind_checker++);
    for(int i = 0; i < n; i++)
        printf("%d ", arr[i]);
	{callgrind_checker--;
    return 0;
	}
}

void find_range(int *arr, int n, int index, int x){
	add_sensor("find_range()", callgrind_checker++);
    int begin, end=-1, count = 0;
    for(int i = (index - 2) < 0 ? 0 : (index-2); i < n ;i++)
    {
        if(arr[i] == x && count == 0)
        {
            begin = i;
            count++;
        } else if(arr[i] == x && count != 0)
            end = i;
    }
    if(end == -1)
        printf("X found in pos: %d\n", begin);
    else
        printf("X found in range: %d-%d\n", begin, end);
	callgrind_checker--;
}

/* Main function */

int binary_search(int *arr, int n, int x)
{
	add_sensor("binary_search()", callgrind_checker++);
    int left = 0;
    int right = n-1;
    int mid = (left+right)/2;
    int index = -1;
    int oper = 0;
    while(left <= right)
    {
        oper++;
        mid = (left + right) / 2;
        if(arr[mid] == x)
        {
            index = mid;
            break;
        } else if (x>arr[mid])
            left = mid + 1;
        else if (x<arr[mid])
            right = mid - 1;
    }
    if(index != -1)
        find_range(arr, n, index, x);
    else
        printf("X not found!\n");

    printf("Iteration count: %d\n", oper);
	{callgrind_checker--;
    return 0;
	}
}

int merge(int *arr, int start, int mid, int end)
{
	add_sensor("merge()", callgrind_checker++);
    int i,j,k;
    int num1 = mid - start + 1;
    int num2 = end - mid;

    /* Temp arrays */
    int arr1[num1], arr2[num2];

    /* Copy data to temp arrays */
    for (i = 0; i < num1; i++)
        arr1[i] = arr[start + i];
    for (j = 0; j < num2; j++)
        arr2[j] = arr[mid + 1 + j];
    
    i = 0;     /* pointer for arr1 */
    j = 0;     /* pointer for arr2 */
    k = start; /* pointer for merged merged arrays */
    while(i < num1 && j < num2)
    {
        if(arr1[i] <= arr2[j])
            arr[k] = arr1[i++];
        else
            arr[k] = arr2[j++];
        k++;
    }

    /* If something remaing, that copy this */
    while (i < num1) {
        arr[k] = arr1[i];
        i++;
        k++;
    }
    /* Similarly but for arr2 */
    while (j < num2) {
        arr[k] = arr2[j];
        j++;
        k++;
    }
	{callgrind_checker--;
    return 0;
	}
}

int merge_sort(int *arr, int start, int end)
{
	add_sensor("merge_sort()", callgrind_checker++);
    if(start < end)
    {
        int mid = (start+end) / 2;
        merge_sort(arr, start, mid);
        merge_sort(arr, mid + 1, end);
        merge(arr, start, mid, end);
    }
	{callgrind_checker--;
    return 0;
	}
}

int main(){
//    if(linux)
//        system("clear");
	add_sensor("main()", callgrind_checker++);
    /* I don't know the command for windows, so windows users will suffer a little ¯\_(ツ)_/¯ */

    /* Generate random sequence. Default seed = 1 */
    srand(time(NULL));

    /* Input parametrs and declaration array */
    int x, n;
    printf("Input n: \n"); scanf("%d", &n);
    printf("Input x: \n"); scanf("%d", &x);
    int arr[FACTOR*n];

    /* Field arr */
    field_arr(arr, FACTOR*n);

    /* Uncommented for buble_sort */
    /* buble_sort(arr, FACTOR*n); */

    /* Merge sort */
    merge_sort(arr, 0 , FACTOR*n-1);

    /* Output arr */
    printf("Sorted array: ");
    print(arr, FACTOR*n);
    printf("\n");

    /* Understand all from function name */
    binary_search(arr, FACTOR*n, x);
    printf("\n");
	{callgrind_checker--;
    return 0;
	}
}
