#ifndef HELLOWORLDC_SCROLLGLIDEHANDLER_H
#define HELLOWORLDC_SCROLLGLIDEHANDLER_H

#include "Point.h"
#include "TouchController.h"
#include "TouchHistory.h"
#include "Accumulator.h"
#include "ScrollState.h"

class ScrollGlideHandler {
private:
    const double scale, friction, minimum, minimumInitial;
    double velocity;
    bool active;
    Accumulator accumulator;

public:
    ScrollGlideHandler(double scale, double friction, double minimum, double minimumInitial);

    void update(TouchHistory history, TouchController &controller, ScrollState scrollState);

private:
    void init(double scrollSpeed);

    void iterate(TouchController &controller);

    void conclude();
};

#endif
