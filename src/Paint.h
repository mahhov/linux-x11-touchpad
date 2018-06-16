#ifndef HELLOWORLDC_PAINT_H
#define HELLOWORLDC_PAINT_H

#include <list>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include "Point.h"

class Paint {
private:
    Display *d;
    Window w;
    XEvent e;
    int s;
    int size, pointSize;
    std::list<Point> points;

public:
    void init(int size, int pointSize);

    void removeAllPoints();

    void addPoint(Point p);

    void repaint();

    static double scale(double value, double factor);
};

#endif
