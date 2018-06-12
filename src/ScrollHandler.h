#ifndef HELLOWORLDC_SCROLLHANDLER_H
#define HELLOWORLDC_SCROLLHANDLER_H

#include "TouchHistory.h"
#include "TouchController.h"
#include "Paint.h"

class ScrollHandler {
private:
    int delta;
    double boundary, threshold;
    bool active;

public:
    ScrollHandler(int delta, double boundary, double threshold);

    void update(TouchHistory history, TouchController &controller, Paint &paint);

private:
    void init(Point movement);

    void iterate(TouchHistory history, TouchController &controller, Paint &paint);

    void conclude();
};

#endif
