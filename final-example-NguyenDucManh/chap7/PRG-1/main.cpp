#include <iostream>
#include "One.h"
using namespace std;

int main()
{
    One ones (2,3);
    cout << "Get X is: " << ones.getX() << "\n";
    cout << "Get Y is: " << ones.getY() << endl;
    return 0;
}
