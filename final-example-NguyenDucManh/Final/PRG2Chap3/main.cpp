#include <iostream>
#include <limits>

using namespace std;

int main ()
{
    long num1 = numeric_limits <long> :: max();
    long num2 = numeric_limits <long> :: min();
    long long first = numeric_limits <long long> :: max();
    long long second = numeric_limits <long long> :: min();

    cout << "The value of maximum long: " << num1 << endl;
    cout << "The value of minimum long: " << num2 << endl;
    cout << "The value of maximum long long: " << first << endl;
    cout << "The value of minimum long long: " << second << endl;

    num1 += 1;
    num2 -= 1;
    first += 1;
    second -= 1;

    cout << "The first value value of maximum long after overflow: " << num1 << endl;
    cout << "The second value of minimum long after overflow: " << num2 << endl;
    cout << "The first value of maximum long long after overflow: " << first << endl;
    cout << "The second value of minimum long long after overflow: " << second << endl;

    return 0;
}
