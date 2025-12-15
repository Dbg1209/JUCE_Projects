#include <iostream>
using namespace std;

int main() {
    cout << "Hello, World!" << endl;
    }

// Pointer Arithmetic

void PointerArith()
{
    int A[] = { 2,4,6,8,10,12 };
    int* p = A;

    // move pointer to next location to print 4
    cout << *p;

    p = p + 3; // pointer will be pointing on 10

    cout << p;  // complete this statement to print 2 without moving pointer
}