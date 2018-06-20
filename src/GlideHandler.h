#ifndef HELLOWORLDC_GLIDEHANDLER_H
#define HELLOWORLDC_GLIDEHANDLER_H

#include "Point.h"
#include "TouchController.h"
#include "TouchHistory.h"

class GlideHandler {
private:
    int delta;
    double scale, friction, minimum;
    Point velocity;
    bool active;

public:
    GlideHandler(int delta, double scale, double friction, double minimum);

    void update(TouchHistory history, TouchController &controller);

private:
    void init(TouchHistory history);

    void iterate(TouchController &controller);

    void conclude();
};

#endif
