#include <iostream>

using namespace std;

int main()
{
    int num;
    cin >> num;
    int reverse_num = 0;
        while (num > 0) {
            reverse_num = reverse_num * 10 + num % 10;
            num = num / 10;
        }
        return reverse_num;
        cout << "reverse of num: " << reverse_num;
    return 0;
}
