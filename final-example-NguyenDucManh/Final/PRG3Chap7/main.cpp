
#include <iostream>
#include <math.h>
using namespace std;
class point
{
    private :
    float x,y, a, b;
public:
    void Input();
    void Output();
    void setXY();
    float Distance( point );

};
void point::Input()
{
    cout<<"Enter x: ";   cin>>x;
    cout<<"Enter y: ";   cin>>y;
}
void point::Output()
{
    cout << "(" << x << "," << y << ")" << "\n";
}
void point::setXY()
{
    cout << "Enter a: "; cin >> a;
    cout << "Enter b: "; cin >> b;
    x = a;
    y = b;
}
float point::Distance(point p2)
{
    return sqrt(float((p2.x- this->x)*(p2.x-this->x)+(p2.y-this->y)*(p2.y-this->y)));
}


int main()
{
    point p1,p2;
    p1.Input();
    p1.Output();
    p2.setXY();
    p2.Output();
    cout << "Distance = " << p1.Distance(p2) << endl;
    return 0;
}
