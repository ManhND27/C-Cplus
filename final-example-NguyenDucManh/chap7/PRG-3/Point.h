#ifndef POINT_H
#define POINT_H

class Podouble {
private:
    double x, y;  // Private data members

public:
   Podouble(double x, double y); // Constructor with default arguments
   double getX() const;  // Getter
   void setX(double x);  // Setter
   double getY() const;
   void setY(double y);
   void setXY(double x, double y);
   void prdouble() const;
   double distance(double x, double y);
};

#endif
