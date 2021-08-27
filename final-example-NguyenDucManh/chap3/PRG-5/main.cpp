#include <iostream>
#include <stdio.h>

using namespace std;

int printDigit(int N)
{
    int r_i;
    for (int i = 1; i <= 3; i++)
    {
        r_i = N % 10;
        N = N / 10;
    cout << r_i << ", ";
    }
}
int main()
{
    int N;
    cout << "Please enter N :";
    cin >> N;
    cout << "The rightmost three digit of number N is :";
    printDigit(N);
    return 0;
}
