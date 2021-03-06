#include <stdio.h>

void printFactors( int );
int isPerfect( int );

int main()
{
    int n;
    for (n=1; n<=1000; n++) {
        if (isPerfect(n)) {
            printFactors(n);
        }
    }
    return 0;
}


void printFactors( int n )
{
    int i=1;
    int sum = 0;

    while(sum < n) {
        sum+=i;
        printf("%d ", i);
        i++;
    }
    printf("= %d\n", n);
}


int isPerfect( int n )
{
    int perfect = 0;
    int i=1;
    int sum = 0;

    while(sum <= n) {
        sum+=i;

        if(sum==n)
            perfect = 1;

        i++;
    }

    return perfect;
}

