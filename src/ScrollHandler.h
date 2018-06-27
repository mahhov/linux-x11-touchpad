#ifndef HELLOWORLDC_SCROLLHANDLER_H
#define HELLOWORLDC_SCROLLHANDLER_H

#include "TouchHistory.h"
#include "TouchController.h"
#include "Paint.h"
#include "Smoother.h"
#include "ScrollState.h"
#include "Accumulator.h"

class ScrollHandler {
private:
    const int delta;
    const double boundary, threshold;
    bool active;
    Smoother changeSmoother;
    Accumulator accumulator;

public:
    ScrollHandler(int delta, double boundary, double threshold, double smoothness);

    ScrollState update(TouchHistory history, TouchController &controller, Paint &paint);

private:
    void init(TouchController &controller, Point movement);

    void iterate(TouchHistory history, TouchController &controller, Paint &paint);

    void conclude(TouchController &controller);

    static ScrollActivity getScrollActivity(bool prevActive, bool active);
};

#endif
