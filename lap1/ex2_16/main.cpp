#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int a = 0;
    int b = 0;

    printf("%s", "Enter two integer numbers: ");
    scanf("%d%d", &a, &b);

    if (b == 0) {
        puts("Unable to divide by zero.");
        return EXIT_FAILURE;
    }

    int sum = a + b;
    int product = a * b;
    int difference = a - b;
    double quoitent = (double)a / b;
    int remainder = a % b;

    printf(
        "Sum is: %d\nProduct is: %d\nDifference is: %d\nQuotient is: %lf\nRemainder is: %d\n",
        sum,
        product,
        difference,
        quoitent,
        remainder);

    return EXIT_SUCCESS;
}
