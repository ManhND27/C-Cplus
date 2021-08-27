#include <iostream>
using namespace std;

int gcd(int m, int n)
{
    if (m == 0)
       return n;
    if (n == 0)
       return m;

    if (m == n)
        return m;

    if (m > n)
        return gcd(m-n, n);
    return gcd(m, n-m);
}

int main()
{
    int m = 9, n = 12;
    cout<<"GCD of "<< m <<" and "<< n <<" is "<<gcd(m, n) << "\n";
    m = 7, n = 11;
    cout<<"GCD of "<< m <<" and "<< n <<" is "<<gcd(m, n) << "\n";
    m = 12, n = 140;
    cout<<"GCD of "<< m <<" and "<< n <<" is "<<gcd(m, n);
    return 0;
}
