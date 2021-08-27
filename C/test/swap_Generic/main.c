#include <stdio.h>

void swap(void *data1Ptr, void *data2Ptr, size_t nbytes);
void swapEnds(void *arr, size_t nelems, size_t elemBytes);
void bubbleSortInt(int *arr, int n);
int main()
{
    /*
    int x = 2;
    int y = 5;
    swap(&x, &y, sizeof (x));
    printf("x : %d y : %d",x,y);
    */
    char nums[] = {'5', '2', '3', '4', '1'};
    size_t nelems = sizeof(nums) / sizeof(nums[0]);
    swapEnds(nums, nelems, sizeof(nums[0]));
    for (int i = 0; i < 5; i++) {
       printf("%d",nums[i]);
    }
    return 0;
}

void swapEnds(void *arr, size_t nelems, size_t elemBytes)
{
   swap(arr, (char *)arr + (nelems -1) * elemBytes, elemBytes);
}

void swap(void *data1Ptr, void *data2Ptr, size_t nbytes)
{
    char temp[nbytes];
    // store a copy of data1 in temporary storage
    memcpy(temp, data1Ptr, nbytes);
    // copy data2 to location of data1
    memcpy(data1Ptr, data2Ptr, nbytes);
    // copy data in temporary storage to location of data2
    memcpy(data2Ptr, temp, nbytes);
}

void bubbleSortInt(int *arr, int n){

}
