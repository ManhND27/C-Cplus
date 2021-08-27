#include <iostream>
#include <stdio.h>

using namespace std;

int main(){
    int number;
    int weeks;
    int days;
    int hours;
    cout << "Please enter the number: ";
    cin >> number;
    weeks = number / 7/ 24;
    int weeks_pr = number % (7*24);
    days = weeks_pr / 24;
    hours = weeks_pr % 24;
    cout << "You entered :";
    cout << number << " Hours" << endl;
    cout << "That is ";
    cout << weeks << " weeks, and ";
    cout << days << " days ,and ";
    cout << hours << " hours" << endl;
    system("pause");
    return 0;
}
