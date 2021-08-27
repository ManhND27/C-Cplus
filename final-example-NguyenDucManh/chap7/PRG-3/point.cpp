#include<math.h>
#include "Point.h"
class Point {
private:
    double x;
    double y;

public:
    Point() {
    }

    Point(double x, double y) {
        this->x = x;
        this->y = y;
    }

    double getX() {
        return x;
    }

    void setX(double x) {
        this->x = x;
    }

    double getY() {
        return y;
    }

    void setY(double y) {
        this->y = y;
    }

    void setXY(double x, double y) {
        this->x = x;
        this->y = y;
    }

    double distance(double x, double y) {
        return sqrt((this->x - x) * (this->x - x) + (this->y - y) * (this->y - y));
    }

    double distance(Point another) {
        return distance(another.getX(), another.getY());
    }
};
