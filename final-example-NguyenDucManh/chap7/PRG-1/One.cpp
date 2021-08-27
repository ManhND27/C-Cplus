#include "One.h"
#include <iostream>
using namespace std;

One::One(int x, int y) : x(x), y(y) { }

int One::getX() const { return x; }
int One::getY() const { return y; }

void One::print() const {
   cout << "One @ (" << x << "," << y << ")";
}

