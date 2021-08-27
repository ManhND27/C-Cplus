#include <iostream>
using namespace std;

class Complex
{
private:
    float real;
    float ima;
public:
    Complex(): real(0), ima(0){ }
    void input()
    {
    cout << "Enter real: ";
    cin >> real;
    cout << "Enter imaginary: ";
    cin >> ima;
    }

    //overload
    Complex operator + (Complex c2)
    {
        Complex temp;
        temp.real = real + c2.real;
        temp.ima = ima + c2.ima;

        return temp;
    }
    Complex operator - (Complex c2)
    {
        Complex temp;
        temp.real = real - c2.real;
        temp.ima = ima - c2.ima;

        return temp;
    }
    Complex operator * (Complex c2)
    {
        Complex temp;
        temp.real = real * c2.real - ima * c2.ima;
        temp.ima = real * c2.ima + c2.real * ima;

        return temp;
    }
    Complex operator / (Complex c2)
    {
        Complex temp;
        temp.real = (real * c2.real + ima * c2.ima) / (c2.real * c2.real + c2.ima * c2.ima);
        temp.ima = (-real * c2.ima + c2.real * ima) / (c2.real * c2.real + c2.ima * c2.ima);

        return temp;
    }
    void output()
    {
        if(ima < 0) {
            cout << "Output Complex number: "<< real << ima << "i" << "\n";
        } else {
            cout << "Output Complex number: " << real << "+" << ima << "i" << "\n";
        }
    }
};

int main()
{
    Complex c1, c2, sum, sub, multi, division;

    cout << "Enter first complex number \n";
    c1.input();

    cout << "Enter second complex number \n";
    c2.input();
    sum = c1 + c2;
    sum.output();
    sub = c1 - c2;
    sub.output();
    multi = c1 * c2;
    multi.output();
    division = c1 / c2;
    division.output();
    return 0;
}
