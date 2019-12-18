/*
	Created by Callgrind ฅ^•ﻌ•^ฅ
	Thu Dec 19 00:35:23 2019
*/
#include "../headers/analyze.h"
#include <iostream>

using namespace std;

float a, b, c;
char d;
int p;
int main()
{
	add_sensor("main()", callgrind_checker++);

    while (p != 2)
    {
        cout << "1 = calculator \n 2 - exit \n";
        cin >> p;
        switch (p)
        {
            case 1:
            {
                cout <<"First number ";
                cin >> a;
                cout << "Action with numbers ";
                cin >> d;
                cout << "Second number ";
                cin >> b;
                if (d == '+')
                    c = a + b;
                if (d == '-')
                    c = a - b;
                if (d == '*')
                    c = a * b;
                if (d == '/')
                    c = a / b;
            }
            case 2:
            {
                break;
            }
        }
        cout << "Result =" << c <<endl;
    }
	callgrind_checker--;
}