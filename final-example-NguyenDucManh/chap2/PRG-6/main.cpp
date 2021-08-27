#include <iostream>

using namespace std;

int main()
{
    float len, area, perimeter;
    cout << "Enter the Length of Square: ";
    cin >> len;
    area = len * len;
    perimeter = len * 4;
    cout << "\nArea = " << area;
    cout << "\nPerimeter = " << perimeter;
    cout << endl;
    return 0;
}
