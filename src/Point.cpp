#include <cmath>
#include <cstdio>
#include "Point.h"

const Point Point::invalidPoint{};

Point::Point(double x, double y) : x(x), y(y), invalid(false), mag(-1) {}

Point::Point() : x(-1), y(-1), invalid(true), mag(-1) {}

Point Point::operator+(Point point) {
    return {x + point.x, y + point.y};
}

Point Point::operator-(Point point) {
    return {x - point.x, y - point.y};
}

Point Point::operator*(double scale) {
    return {x * scale, y * scale};
}

Point Point::operator/(double scale) {
    return {x / scale, y / scale};
}

double Point::operator%(Point point) {
    return x * point.x + y * point.y;
}

double Point::operator*(Point point) {
    return x * point.y - y * point.x;
}

Point Point::operator++() {
    return {-y, x};
}

Point Point::operator--() {
    return {y, -x};
}

double Point::operator!() {
    if (mag == -1)
        mag = sqrt(x * x + y * y);
    return mag;
}

void Point::debugPrint() {
    printf("<%f, %f>\n", x, y);
}
