#include "circle.h"

double Circle :: getRadius () const
{
return radius;
}

double Circle :: getArea () const
{
const double PI = 3.14;
return (PI * radius * radius);
}

double Circle :: getPerimeter () const
{
const double PI = 3.14;
return (2 * PI * radius);
}

void Circle :: setRadius (double value)
{
radius = value;
}
