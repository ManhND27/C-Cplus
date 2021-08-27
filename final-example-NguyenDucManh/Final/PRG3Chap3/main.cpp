#include <iostream>
#include <limits>

using namespace std;

int main ()
{
    float a = numeric_limits <float> :: max();
    float b = numeric_limits <float> :: min();
    double first = numeric_limits <double> :: max();
    double second = numeric_limits <double> :: min();

    cout << "The value of maximum float: " << a << endl;
    cout << "The value of minimum float: " << b << endl;
    cout << "The value of maximum double: " << first << endl;
    cout << "The value of minimum double: " << second << endl;

    a *= 1000.00;
    b *= 1000.00;
    first *= 1000.00;
    second *= 1000.00;

    cout << "The first value value of maximum float after overflow: " << a << endl;
    cout << "The second value of minimum float after overflow: " << b << endl;
    cout << "The first value of maximum double after overflow: " << first << endl;
    cout << "The second value of minimum double after overflow: " << second << endl;

    return 0;
}
