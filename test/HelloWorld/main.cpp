#include <iostream>

int main()
{
    int n;
    printf("Nhap so phan tu : ");
    scanf("%d",&n);
    if (n <= 0) {
        return 0;
    }

    int min = 0;
    for (int i=1; i<=n; i++) {
        int temp;
        printf("Nhap phan tu tu %d : ",i);
        scanf("%d",&temp);
        if (temp < min) {
            min = temp;
        }
    }
    printf("Gia tri nho nhat %d :",min);
    return 0;
}
