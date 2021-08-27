#include <iostream>

using namespace std;

int main ()
{
    int n;
    int firstDigit;
    int secondDigit;
    cin >> n;
    firstDigit = n % 10 ;
    n = n / 10;
    secondDigit = n % 10 ;

    cout << "Extracted second digit: " << secondDigit << endl;

    return 0;
}
