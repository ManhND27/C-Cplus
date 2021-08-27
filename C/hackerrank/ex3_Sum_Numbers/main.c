#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main()
{
    int n1;
    int n2;
    float n3;
    float n4;
    scanf("%d %d", &n1, &n2);
    scanf("%f %f", &n3, &n4);
    int n12 = n1 + n2;
    int n21 = n1 - n2;
    float n34 = n3 + n4;
    float n43 = n3 - n4;
    printf("%d %d\n", n12, n21);
    printf("%.1f %.1f", n34, n43);
    return 0;
}
