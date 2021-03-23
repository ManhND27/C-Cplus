#include <stdio.h>

int main()
{
    int count,num;
    int smallest = 0;

    printf("Enter the number of values to be entered : ");
    scanf("%d",&count);
    if (count > 0) {
        printf("Enter number : ");
        scanf("%d",&smallest);
    }

    while (count-- >= 1) {
        printf("Creat nb ");
        scanf("%d",&num);
        if (num < smallest) {
            smallest = num;
        }
        printf("\nThe smallest numbers is : %d",smallest);

    return 0;
    }

}
