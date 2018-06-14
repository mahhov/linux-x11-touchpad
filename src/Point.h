#ifndef HELLOWORLDC_POINT_H
#define HELLOWORLDC_POINT_H

struct Point {
    static const Point invalidPoint;

    bool invalid;
    double x, y;
    double mag;

    Point(double x, double y);

    Point operator+(Point point);

    Point operator-(Point point);

    Point operator*(double scale); // scale

    Point operator/(double scale); // inverse scale

    double operator%(Point point); // dot product

    double operator*(Point point); // cross product magnitude

    Point operator++(); // clockwise rotation

    Point operator--(); // counter clockwise rotation

    double operator!(); // magnitude

    void debugPrint();

private:
    Point(); // invalid constructor
};

#endif
