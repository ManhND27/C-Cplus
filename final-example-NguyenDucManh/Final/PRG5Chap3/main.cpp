#include <iostream>

using namespace std;

int main ()
{
    int n;
    int firstDigit;
    cin >> n;
    for(int i = 0; i < 3 ;i++) {
        firstDigit = n % 10 ;
        n = n / 10;
        cout << firstDigit << endl;
    }
    return 0;
}
