#include <stdio.h>
#include <stdlib.h>

int main()
{
   int nb1=0;
   int nb2=0;
   int nb3=0;
    printf("%s", "Enter three different integers: ");
    scanf("%d%d%d", &nb1, &nb2, &nb3);

    int sum = nb1 + nb2 + nb3;
    int average = sum / 3;
    int product = nb1 * nb2 * nb3;

    int smallest = nb1;
    if (smallest >  nb2) {
        smallest = nb2;
    }
    if (smallest > nb3) {
        smallest = nb3;
    }

    int largets = nb1;
    if (largets < nb2) {
        largets = nb2;
    }
    if (largets < nb3) {
        largets = nb3;
    }

    printf("Sum is: %d\n Average is: %d\nProduct is: %d\nSmallest number is: %d\nLargest number is: %d\n",sum,average,product,smallest,largets);

    return  EXIT_SUCCESS;
}
