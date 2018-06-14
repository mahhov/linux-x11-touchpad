#ifndef HELLOWORLDC_SCROLLHANDLER_H
#define HELLOWORLDC_SCROLLHANDLER_H

#include "TouchHistory.h"
#include "TouchController.h"
#include "Paint.h"
#include "Smoother.h"

class ScrollHandler {
private:
    int delta;
    double boundary, threshold;
    bool active;
    Point center;
    Smoother smoother;
    double scrollFraction;

public:
    ScrollHandler(int delta, double boundary, double threshold, double smoothness);

    void update(TouchHistory history, TouchController &controller, Paint &paint);

private:
    void init(Point movement);

    void iterate(TouchHistory history, TouchController &controller, Paint &paint);

    void conclude(TouchController &controller);
};

#endif
