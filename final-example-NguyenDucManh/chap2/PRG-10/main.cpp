#include <iostream>

using namespace std;

int main()
{
    string stNumber, stCity, stZipcode;
    cout << "Enter street-number :" << endl;
    cin >> stNumber;
    cout << "Enter street city: " << endl;
    cin >> stCity;
    cout << "Enter state zip-code: " << endl;
    cin >> stZipcode;
    cout << endl;
    cout << stNumber << "," << stCity << "," << stZipcode <<endl;
    return 0;
}
