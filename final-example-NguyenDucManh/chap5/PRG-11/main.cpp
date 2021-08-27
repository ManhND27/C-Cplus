#include<iostream>
using namespace std;
int main ()
{
    int arr[100], n, i, max, min;
    cout << "Enter the size of the array : ";
    cin >> n;
    cout << "Enter the elements of the array with element <= 10: ";
    for (i = 0; i < n; i++)
    {
        cin >> arr[i];
        int k = arr[i];
        if(k > 10)
        {
            cout<< "Enter number of elements(1 to 10)" <<endl;
            cin>>arr[i];
        }
    }
    max = arr[0];
    for (i = 0; i < n; i++)
    {
        if (max < arr[i])
            max = arr[i];
    }
    min = arr[0];
    for (i = 0; i < n; i++)
    {
        if (min > arr[i])
            min = arr[i];
    }
    cout << "Largest element : " << max << "\n";
    cout << "Smallest element : " << min;
    return 0;
}
