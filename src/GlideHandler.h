#ifndef HELLOWORLDC_GLIDEHANDLER_H
#define HELLOWORLDC_GLIDEHANDLER_H

#include "Point.h"
#include "TouchController.h"
#include "TouchHistory.h"

class GlideHandler {
private:
    double scale, friction, minimum;
    double x, y;
    bool active;

public:
    GlideHandler(double scale, double friction, double minimum);

    void update(TouchHistory history, TouchController &controller);

private:
    void init(Point movement);

    void iterate(TouchController &controller);

    void conclude();
};

#endif
