#include "Two.h"
#include <iostream>
using namespace std;

Two::Two(int x, char a) : x(x), a(a) { }

int Two::getX() const { return x; }
char Two::getA() const { return a; }

void Two::setX(int x) { this->x = x; }
void Two::setA(char a) { this->a = a; }
void Two::print() const {
   cout << "Two @ (" << x << "," << a << ")";
}

