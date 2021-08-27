#include <iostream>
#include <climits>
using namespace std;

int main()
{
    cout << "Maximum value and Minimum value of short in C++" << endl;
    short int valueLimitMax = SHRT_MAX;
    short int valueLimitMin = SHRT_MIN;
    cout << "Maximum :" << valueLimitMax << "\n";
    cout << "Minimum :" << valueLimitMin << "\n";
    unsigned int valueFromLimits = UINT_MAX;
    cout << "Value limit of unsigned int in C is :" << valueFromLimits << "\n";
    return 0;
}
