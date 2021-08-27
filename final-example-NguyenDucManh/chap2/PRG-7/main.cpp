#include <iostream>
#include <string>
using namespace std;

int main()
{
    char fname[30], lname[30];
    cout << " Input First Name: ";
    cin >> fname;
    cout << " Input Last Name: ";
    cin >> lname;
    cout << "Your full name is :" << lname << "," << fname << endl;
    return 0;
}
