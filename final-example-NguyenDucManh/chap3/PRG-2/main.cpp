#include <iostream>
#include <climits>
using namespace std;

int main()
{
    long valueLimitMin = LONG_MAX;
    long valueLimitMax = LONG_MIN;
    cout << "Maximum value and Minimum value of long in C++" << endl;
    cout << "Maximum long:" << valueLimitMax << "\n";
    cout << "Minimum long:" << valueLimitMin << "\n";
    long long valueLimitLLMax = LLONG_MAX;
    long long valueLimitLLMin = LLONG_MIN;
    cout << "Maximum value and Minimum value of long long in C++" << endl;
    cout << "Maximum long long:" << valueLimitLLMax << "\n";
    cout << "Minimum long long:" << valueLimitLLMin << "\n";
    return 0;
}
