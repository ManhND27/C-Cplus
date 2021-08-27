#include "Two.h"
#include <iostream>
using namespace std;

int main() {

   Two p1;
   Two p2(2, 'a');
   p1.print();
   cout << endl;
   p2.print();
   cout << endl;
   Two * ptrP3, * ptrP4;
   ptrP3 = new Two();
   ptrP4 = new Two(4, 'b');
   ptrP3->print();
   cout << endl;
   ptrP4->print();
   cout << endl;

   delete ptrP3;
   delete ptrP4;
   Two & p5 = p2;
   p5.print();
   cout << endl;
}
