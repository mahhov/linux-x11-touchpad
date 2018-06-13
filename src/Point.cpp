#include <cmath>
#include <cstdio>
#include "Point.h"

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
    return sqrt(x * x + y * y);
}

void Point::debugPrint() {
    printf("<%f, %f>\n", x, y);
}
