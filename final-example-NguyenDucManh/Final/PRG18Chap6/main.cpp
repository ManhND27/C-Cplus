#include <iostream>

using namespace std;
void Pi(int n) {
    double pi = 0;
    for (int i = 0; i < n; i++) {
        if (i % 2 == 1) {
            pi = pi - ((double)1/(2*i+1));
        } else {
            pi = pi + ((double)1/(2*i+1));
        }

    }
    cout << pi*4;
}
int main()
{
    int n;
    cin >> n;
    Pi(n);
    return 0;
}
