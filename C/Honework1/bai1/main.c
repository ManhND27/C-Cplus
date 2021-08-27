#include <stdio.h>

void swap(int* xp, int* yp);
void printArray(int array[], int arrSize);
int main()
{
    int n;
    int arr[n];
    printf("Input length arr : ");
    scanf("%d",&n);
    printf("Input element in arr : ");
    for (int i = 0; i < n; i++) {
        scanf("%d",arr[i]);
    }
    printArray(arr,n);
    return 0;
}

void printArray(int array[], int arrSize)
{
    int i;
    for (i = 0; i < arrSize; i++) {
        printf("%d ", array[i]);
    }

    printf("\n");
}

void swap(int *left, int *right)
{
    int temp = *left;
    *left = *right;
    *right = temp;
}

/*
Selection Sort
    Ý tưởng :
            - Chọn phần tử nhỏ nhất đưa về vị trí đầu tiên của mảng hiện tại và không cần quan tâm đến nó nữa.
            - Khi đó mảng chỉ còn lại n - 1 phần tử của mảng ban đầu, tiếp tục xét từ phần tử thứ 2 của mảng.
            - Lặp lại cho đến khi dãy hiện tại chỉ còn 1 phần tử.
*/

void selectionSort(int array[], int arrSize)
{
    int i;
    for (i = 0; i < arrSize - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < arrSize; j++) {
            if (array[minIndex] > array[j]) {
                minIndex = j;
            }
        }
        swap(&array[minIndex], &array[i]);
    }
}
/*
Bubble Sort
Ý tưởng rất đơn giản:

- Ta lặp qua từng phần tử một
    - Tại một phần tử
        - Ta lặp qua các phần tử còn lại
            - Nếu phần tử nào trong các phần tử còn lại mà nhỏ hơn phần tử hiện tại thì thay thế phần tử hiện tại bằng phần tử nhỏ hơn đó

Insertion Sort
Ý tưởng:

- Ta lặp qua từng phần tử một:
    - Tại một phần tử
        - Ta lặp qua các phần tử ở trước phần tử này:
            - Tìm vị trí mà phần tử này lớn hơn các phần tử bên trên và nhỏ hơn các phần tử bên phải
            - Di chuyển tất cả các phần tử bên phải lên một bậc
            - Đặt phần tử này vào vị trí đó (Đây chính là ý nghĩa Insertion(chèn))

Quick Sort
Ý Tưởng :
    - Chọn phần tử chốt.
    - Khai báo 2 biến con trỏ để trỏ để duyệt 2 phía của phần tử chốt.
    - Biến bên trái trỏ đến từng phần tử mảng con bên trái của phần tử chốt.
    - Biến bên phải trỏ đến từng phần tử mảng con bên phải của phần tử chốt.
    - Khi biến bên trái nhỏ hơn phần tử chốt thì di chuyển sang phải.
    - Khi biến bên phải nhỏ hơn phần tử chốt thì di chuyển sang trái.
    - Nếu không xảy ra trưởng hợp 5 và 6 thì tráo đổi giá trị 2 biến trái và phải.
    - Nếu trái lớn hơn phải thì đây là giá trị chốt mới.
*/
