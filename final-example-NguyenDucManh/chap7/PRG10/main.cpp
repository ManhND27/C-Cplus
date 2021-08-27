#include <iostream>

using namespace std;
class Complex
{
public:
    float real;
    float ima;
    void Input()
    {
        cin >> real;
        cin >> ima;
    }
    void Output()
    {
        cout << real << " + " << ima << "i " << endl;
    }
    void sum(Complex x, Complex y)
    {
        real = x.real + y.real;
        ima = x.ima + y.ima;
    }
    void subtract(Complex x, Complex y)
    {
        real = x.real - y.real;
        ima = x.ima - y.ima;
    }
    void multi(Complex x, Complex y)
    {
        real = x.real * y.real - x.ima * y.ima;
        ima = x.real * y.ima + y.real * x.ima;
    }
    void division(Complex x, Complex y)
    {
        real = (x.real * y.real + x.ima * y.ima) / (y.real * y.real + y.ima * y.ima);
        ima = (-x.real * y.ima + y.real * x.ima) / (y.real * y.real + y.ima * y.ima);
    }
};


int main()
{
    Complex x, y, c;
    cout << "Enter first complex: " <<endl;
    x.Input();
    cout << "Enter second complex: " <<endl;
    y.Input();
    cout << "Sum: ";
    c.sum(x,y);
    c.Output();
    cout << "Subtract: ";
    c.subtract(x,y);
    c.Output();
    cout << "Multi: ";
    c.multi(x,y);
    c.Output();
    cout << "Division: ";
    c.division(x,y);
    c.Output();
    return 0;
}
