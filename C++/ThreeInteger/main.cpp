#include <iostream>

using namespace std;

int main()
{
    cout << "Num 1 : " << endl;
    int num1;
    cin >> num1;

    cout << "Num 2 : " << endl;
    int num2;
    cin >> num2;

    cout << "Num 3 : " << endl;
    int num3;
    cin >> num3;

    int min = num1;
    if (num2 < min) {
        cout << "Min is num2 : "<< num2;
    }
    else if(num3 < min){
        cout << "Min is num3 : "<< num3;
    }
    else
         cout << "Min is num1 : "<< num1;

    return 0;
}
