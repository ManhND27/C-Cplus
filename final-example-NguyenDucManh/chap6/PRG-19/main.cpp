#include <iostream>
#include<time.h>
using namespace std;

int main()
{
    int a[100];
    for (int i = 0; i < 100; i++ )
    {
        a[i] = rand()%1000;
        for(int j = 1; j <= 3; j++)
        {
            if ((a[i] % (10*j)) == 0) {
                a[i] += 1;
            }
        }
        if (i%10==0) {
        cout << "\n";
        }
        cout << "   "<<a[i];
    }
    return 0;
}
