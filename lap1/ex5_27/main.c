#include <stdio.h>
#include <math.h>

int main()
{
    int a=0;
    for (int i=1; i<=1000; i++) {
        if (isPrime(i))
            a++;
    }
    printf("%d sum of prime number\n",a);
    return 0;
}

int isPrime(int n)
{
    int hight  = (int)sqrt(n);
    int i;

    if (n<=2) {
        return 0;
    }

    if (n%2 == 0) {
        return 0;
    }

    for (i=3; i<= hight; i+=2) {
        if (n%i==0)
            return 0;
    }

    printf("%d is a prime number\n", n);
    return 1;
}
