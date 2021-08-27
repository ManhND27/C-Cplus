#include <iostream>
#include <climits>
#include <limits>
#include <float.h>
using namespace std;

int main()
{
    float valueLimitMax = FLT_MAX;
    float valueLimitMin = FLT_MIN;
    cout << "Maximum value and Minimum value of float in C++" << endl;
    cout << "Maximum float:" << valueLimitMax << "\n";
    cout << "Minimum float:" << valueLimitMin << "\n";
    double valueLimitDmax = DBL_MAX;
    double valueLimitDmin = DBL_MIN;
    cout << "Maximum value and Minimum value of double in C++" << endl;
    cout << "Maximum double: " << valueLimitDmax << "\n";
    cout << "Minimum double: " << valueLimitDmin << "\n";
    return 0;
}
