// CppRecap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

float Area(int b, int h)
{
    //int b, h;

    float area;

    area = (float)(b * h) / 2;

    return area; 
}

void PointerArith()
{
    int A[] = { 2,4,6,8,10,12 };
    int *p = A;

    cout << "Pointer Arith Exercise \n" << *p << endl;

    p++;

    p = p + 3;

    cout << *p << endl;

    cout << p[-4] << endl;
}




int main()
{
    cout << "Hello World!\n";

    int b = 5, h = 7;

    float area;

    area = (float)(b * h) / 2;

    cout << area << endl;
    cout << Area(10, 7) << endl;

    PointerArith();
    
   }
