#include <iostream>

using namespace std;

int main ()
{
    unsigned long time;
    unsigned long days;
    unsigned long weeks;
    unsigned long hours ;
    cin >> time;

    days = time / 24;
    weeks = (time - (days * 24)) / 7;
    hours = time - (days * 24) - (weeks * 7);

    cout << "Given time in hours: " << time << endl;
    cout << "Result is: ";
    cout << days << " days, ";
    cout << weeks << " weeks, ";
    cout << hours << " hours." << endl;

    return 0;
}
