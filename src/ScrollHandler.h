#ifndef HELLOWORLDC_SCROLLHANDLER_H
#define HELLOWORLDC_SCROLLHANDLER_H

#include "TouchHistory.h"
#include "TouchController.h"
#include "Paint.h"
#include "Smoother.h"

const int NOT_SCROLLING = -1;
const int SCROLLING = -2;

class ScrollHandler {
private:
    int delta;
    double boundary, threshold;
    bool active;
    Point center;
    Smoother smoother;
    double scrollFraction;
    double lastScroll;

public:
    ScrollHandler(int delta, double boundary, double threshold, double smoothness);

    double update(TouchHistory history, TouchController &controller, Paint &paint);

private:
    void init(Point movement);

    void iterate(TouchHistory history, TouchController &controller, Paint &paint);

    void conclude(TouchController &controller);
};

#endif
