#include <iostream>
using namespace std;

int main()
{
    for (int year = 2000; year <= 2099; year++) {
        cin >> year;
        if (year % 4 == 0) {
            if (year % 100 == 0) {
                if (year % 400 == 0) {
                    cout << year << ": leap year";
                } else {
                    cout << year << ": not leap year";
                }

            } else {
                cout << year << ": leap year";
            }
        } else {
            cout << year << ": not leap year";
        }
    }
    return 0;
}
