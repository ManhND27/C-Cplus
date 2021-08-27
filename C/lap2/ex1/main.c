#include <stdio.h>
#include <math.h>

#define PRIMES 100000

//create arr
int *createArray(int n);
//chage arr to arr[prime]=-1
void erastosthemes(int *arr, int size);
//free allocate memory of valirable
void freeArray(int **arr);

void printPrimes(int *arr, int size);

int main(int argc, char *argv[])
{
    int n = 100;
    int *arr = createArray(n);
    erastosthemes(&arr,n);
}

int *createArray(int n)
{
    int *NewArr = malloc(sizeof(int) * n);
    for(int i = 0; i < n; i++)
    {
            NewArr[i] = i + 2;
    }
    return NewArr;
}

void erastosthemes(int *arr, int size){
    int j = 0;
    *arr = (int*)malloc(sizeof(int) * size);
    for (int i=0; i < size;i++){
           if (arr[i]!=-1){
               for (int j=arr[i]-2;j<size;j+=arr[i])
                   arr[j]=-1;
           }
       }
}

void freeArray(int **arr)
{

}

void printPrimes(int *arr, int size)
{

}



