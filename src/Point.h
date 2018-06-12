#ifndef HELLOWORLDC_POINT_H
#define HELLOWORLDC_POINT_H

struct Point {
    double x, y;

    Point operator+(Point point);

    Point operator-(Point point);

    Point operator*(double scale); // scale

    Point operator/(double scale); // inverse scale

    double operator*(Point point); // cross product magnitude

    Point operator++(); // clockwise rotation

    Point operator--(); // counter clockwise rotation

    double operator!(); // magnitude

    void debugPrint();
};

#endif
