#include "Paint.h"

#include <iostream>

void Paint::init(int size, int pointSize) { // todo use : init
    this->size = size;
    this->pointSize = pointSize;
    d = XOpenDisplay(NULL);
    s = DefaultScreen(d);
    w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, size, size, 1, BlackPixel(d, s), WhitePixel(d, s));
    XSelectInput(d, w, ExposureMask | KeyPressMask);
    XMapWindow(d, w);
    XNextEvent(d, &e);
}

void Paint::removeAllPoints() {

    points.clear();
}

void Paint::addPoint(Point p) {
    points.push_back(p);
}

void Paint::repaint() {
    XClearWindow(d, w);
    for (auto &point : points)
        XFillRectangle(d, w, DefaultGC(d, s), point.x * (size - pointSize), point.y * (size - pointSize), pointSize, pointSize);
    XSync(d, true);
}

double Paint::scale(double value, double factor) {
    double scaled = -value * factor + .5f;
    if (scaled < 0)
        scaled = 0;
    if (scaled > 1)
        scaled = 1;
    return scaled;
}
