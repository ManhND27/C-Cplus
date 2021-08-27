#include <iostream>
#include "fraction.h"
using namespace std;

int main()
{
    Fraction *fract1 = new Fraction() ;
    Fraction *fract2 =  new Fraction(14, 21);
    Fraction *fract3 = new Fraction(11, -8);
    Fraction *fract4 = new Fraction(*fract3);

    // Printing the object
    cout << "Printing four fractions after constructed: " << endl;
    cout << "fract1: ";
    fract1-> print();
    cout << "fract2: ";
    fract2-> print();
    cout << "fract3: ";
    fract3-> print();
    cout << "fract4: ";
    fract4-> print();
    // Using mutators
    cout << "Changing the first two fractions and printing them:" << endl;
    fract1->setNumer(4);
    cout << "fract1: ";
    fract1->print();
    fract2->setDenom(-5);
    cout << "fract2: ";
    fract2->print();
    // Using accessors
    cout << "Testing the changes in two fractions:" << endl;
    cout << "fract1 numerator: " << fract1->getNumer() << endl;
    cout << "fract2 numerator: " << fract2->getDenom() << endl;

    delete  fract1;
    fract1 = NULL;

    delete  fract2;
    fract2 = NULL;

    delete  fract3;
    fract3 = NULL;

    delete  fract4;
    fract4 = NULL;

    return 0;
}
