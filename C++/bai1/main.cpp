#include <iostream>

using namespace std;

int main()
{
    int year;
    bool divBy400, divBy4, divBy100;
    bool leapYear;
    // Input year
    cout << "Enter the year: ";
    cin >> year;
    // Set conditions
    divBy400 = ((year % 400) == 0);
    divBy4 = ((year % 4) == 0);
    divBy100 = ((year % 100) == 0);
    leapYear = (divBy400) || (divBy4 && !(divBy100));
    // Decision and output
    if (leapYear)
    {
    cout << "Year " << year << " is a leap year." << endl;
    }
    else
    {
    cout << "Year " << year << " is not a leap year." << endl;
    }
    return 0;
}
