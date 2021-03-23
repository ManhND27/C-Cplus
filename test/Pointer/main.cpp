#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
int main(int argc, char *argv[])
{
int x = 351;
int *ptr; // ptr is a pointer to a int
ptr = &x; // ptr now contains the address of x
printf("&x is %p\n", &x);
printf(" ptr is %p\n", ptr);
printf(" x is %d\n", x);
*ptr = 333; // change the value of x
printf(" x is %d\n", x);
return EXIT_SUCCESS;
}
