#include <iostream>

using namespace std;
void PascalsTriangle (int n){
    for(int i = 0; i < n; i++)
    {
        int x = 1;
        for(int j = 0; j <= i; j++)
        {
            cout << x << " ";
            x = x * (i - j) / (j + 1);
        }
        cout << endl;
    }
}
int main()
{
    int n;
    cout << "Enter the number of rows you would like to print for Pascal's Triangle: ";
    cin >> n;
    PascalsTriangle(n);
    return 0;
}
