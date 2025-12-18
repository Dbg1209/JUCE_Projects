// CppRecap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

float Area(int b, int h)
{
    float area = static_cast<float>(b * h) / 2;
    return area;
}

void PointerArith()
{
    int A[] = { 2, 4, 6, 8, 10, 12 };
    int* p = A;

    std::cout << "Pointer Arith Exercise \n" << *p << std::endl;

    p++;
    p = p + 3;

    std::cout << *p << std::endl;
    std::cout << p[-4] << std::endl;
}

void LengthOfString(std::string str)
{
    int count = 0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        count++;
    }

    std::cout << "Length of " << str << " is " << count << std::endl;
}

void LengthWithIterator(std::string str)
{
    std::string::iterator it;
    int count = 0;

    for (it = str.begin(); it != str.end(); it++)
    {
        count++;
    }

    std::cout << "Length of " << str
        << " with iterator is " << count << std::endl;
}

void UpperToLower(std::string& str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] >= 'A' && str[i] <= 'Z') //'A' and 'Z' can be swapped by their ASCII values which were 'A' = 65, 'Z' = 90
        {
            str[i] = str[i] + 32;
        }

        std::cout << str[i];
    }
}

void PascalTriangle()
{
    int totalRows;
    int currentRow;

	std::cout << "Enter a whole number between 1 and 20 to for a Pascal's Triangle: ";
	std::cin >> totalRows;
    

    if (totalRows >= 1 || totalRows <= 20)
    {
        int numToDisplay[21][21];

        for (int i = 0; i <= totalRows; i++)
        {
            numToDisplay[i][0] = 1;
            for (int j = 1; j < i; j++)
            {
                numToDisplay[i][j] = numToDisplay[i - 1][j - 1] + numToDisplay[i - 1][j];
            }
            numToDisplay[i][i] = 1;
	    }

            
        for (currentRow = 0; currentRow <= totalRows; currentRow++)
        {
            int spaces = (totalRows - currentRow);
            for (int s = 0; s < spaces; s++)
            {
                std::cout << ' ';
            }
            for (int k = 0; k <= currentRow; k++)
            {
                std::cout << numToDisplay[currentRow][k] << ' ';

            }
            std::cout << std::endl;
        }
    }
    else 
    {
        std::cout << "That's not a number between 1 and 20, buddy." << std::endl;
	}
}



int main()
{
    std::cout << "Hello World!\n";

    int b = 5, h = 7;

    float area = static_cast<float>(b * h) / 2;

    std::cout << area << std::endl;
    std::cout << Area(10, 7) << std::endl;

    PointerArith();

    std::cout << std::endl << "String Exercises" << std::endl;

    std::cout << "Enter a String to know it's length" << std::endl;

    LengthOfString("Hard Coded to skip");

    std::cout << "Now with string::iterator, enter another string" << std::endl;
    LengthWithIterator("Hard coded too");

    std::cout << std::endl << std::endl
        << "Changing upper case to lower case on string" << std::endl;

    std::cout << "Lets enter a string in upper case (or combined) "
        << "to transform to lower case... " << std::endl;

    std::string strUp2Low;
    //std::getline(std::cin, strUp2Low);
    std::cout << std::endl;

    UpperToLower(strUp2Low);


    std::cout << "Print Pascal's Triangle" << std::endl;

    PascalTriangle();

    return 0;
}
