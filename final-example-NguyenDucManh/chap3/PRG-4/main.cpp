#include <iostream>
#include <stdio.h>

using namespace std;

int printDigit(int N)
{
    int r;
    while(N > 10)
    {
        r = N % 10;
        N = N / 10;
    }
    cout << r << "\n";
}
int main()
{
    int N;
    cout << "Please enter N :";
    cin >> N;
    cout << "Second digit of number N is :";
    printDigit(N);
    return 0;
}
