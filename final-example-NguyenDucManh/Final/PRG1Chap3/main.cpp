#include <iostream>
#include <limits>

using namespace std;

int main ()
{
    short a = numeric_limits <short> :: max();
    short b = numeric_limits <short> :: min();
    unsigned int first = numeric_limits <unsigned int> :: max();
    unsigned int second = numeric_limits <unsigned int> :: min();

    cout << "The value of maximum short: " << a << endl;
    cout << "The value of minimum short: " << b << endl;
    cout << "The value of maximum unsigned int: " << first << endl;
    cout << "The value of minimum unsigned int: " << second << endl;

    a += 1;
    b -= 1;
    first += 1;
    second -= 1;

    cout << "The first value value of maximum short after overflow: " << a << endl;
    cout << "The second value of minimum short after overflow: " << b << endl;
    cout << "The first value of maximum unsigned int after overflow: " << first << endl;
    cout << "The second value of minimum unsigned int after overflow: " << second << endl;

    return 0;
}
