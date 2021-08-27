#include <iostream>
#include <vector>

#include "circle.h"
using namespace std;

void swapPointer(int *left, int *right);
void swapReference(int &left, int &right);
void swapValue(int left, int right);

int main()
{
    std::cout
    Circle circle1;
    std::cout << "Radius: " << circle1.getRadius() << std::endl;
    cout << "Area: " << circle1.getArea() << endl;
    cout << "Perimeter: " << circle1.getPerimeter() << endl << endl;
    // Instantiation of circle2 and applying operations on it
    Circle circle2 (circle1);
    cout << "Radius: " << circle2.getRadius() << endl;
    cout << "Area: " << circle2.getArea() << endl;
    cout << "Perimeter: " << circle2.getPerimeter() << endl << endl;
    // Instantiation of circle3 and applying operations on it
    Circle circle3;
    cout << "Radius: " << circle3.getRadius() << endl;
    cout << "Area: " << circle3.getArea() << endl;
    cout << "Perimeter: " << circle3.getPerimeter() << endl << endl;
    // Calls to destructors occur here
    }

   int a = 5;
   int b = 10;


   cout << "Before swapPointer : " << "a = " << a << " b = " << b << endl;

//   swapPointer(&a, &b);
//   swapReference(a, b);
   swapValue(a ,b);

   cout << "Before swapPointer : " << "a = " << a << " b = " << b << endl;

   vector<int> vec(10);

   for (int i =  0; i < 10; i++) {
       vec.at(i) = i * i;
   }

   cout << "Regular navigation ";
   vector<int>::iterator iter;
   for (iter = vec.begin(); iter != vec.end(); ++iter) {
       cout << *iter << " ";
   }
   return 0;
}

void swapPointer(int *left, int *right)
{
    int temp = *left;
    *left = *right;
    *right = temp;
}

void swapReference(int &left, int &right)
{
    int temp = left;
    left = right;
    right = temp;
}

void swapValue(int left, int right)
{
    int temp = left;
    left = right;
    right = temp;
}
