#ifndef CIRCLE_H
#define CIRCLE_H


class Circle
{
private: // access modifier
    double radius;

protected:
    char haveRice;

public:
    void setRadius (double radius); // Mutator
    double getRadius () const; // Accessor
    double getArea () const; // Accessor
    double getPerimeter () const; // Accesso

};

#endif // CIRCLE_H
