#include "ScrollHandler.h"

double scale(double value, double factor) {
    double scaled = -value * factor + .5f;
    if (scaled < 0)
        scaled = 0;
    if (scaled > 1)
        scaled = 1;
    return scaled;
}

ScrollHandler::ScrollHandler(int delta, double boundary, double threshold) :
        delta(delta),
        boundary(boundary),
        threshold(threshold),
        active(false) {}

void ScrollHandler::update(TouchHistory history, TouchController &controller, Paint &paint) {
    if (history.getState() == PRESSED)
        init(history.getLastPoint());
    else if (history.getState() == RELEASED)
        conclude();
    iterate(history, controller, paint);
}

void ScrollHandler::init(Point movement) {
    if (active = movement.x > boundary)
        center = {movement.x - .03, movement.y}; // todo is this needed? if so, make .03 a const
}

void ScrollHandler::iterate(TouchHistory history, TouchController &controller, Paint &paint) {
    if (active) {
        Point base = history.getPastPoint(delta);
        Point last = history.getLastPoint();
        Point delta = last - base;

        Point relativeBase = base - center;
        double relativeBaseMag = !relativeBase;
        if (!relativeBaseMag)
            return;

        double dot = relativeBase % delta - .0001; // todo make constant
        double cross = relativeBase * delta;

        Point centerShift = dot > 0 ^ cross > 0 ? ++delta : --delta;
        center = last + centerShift / !centerShift * .2;

        double change = -cross / relativeBaseMag;

        int intChange = 0; // todo extract
        if (change > threshold)
            intChange = 1;
        else if (change < -threshold)
            intChange = -1;

        paint.addPoint({.1, scale(change, 5)});
        paint.addPoint({.15, scale(threshold, 5)});
        paint.addPoint({.15, scale(-threshold, 5)});
        paint.addPoint({.2, scale(intChange, .3)});

        paint.addPoint(last);
        Point fakeBase = last - delta * .1 / !delta;
        paint.addPoint(fakeBase);
        paint.addPoint(center);

        paint.addPoint({.3, scale(dot, 500)});
        paint.addPoint({.35, scale(cross, 30)});

        // todo support slower scrolling speed based on finger movement
        // todo support for very fast finger getting sparse data
//        controller.scroll(intChange);

        for (int a = 1; a < 20; a++)
            paint.addPoint(history.getPastPoint(a));

    }
    paint.addPoint({boundary, 0});
    paint.addPoint({boundary, 1});
}

void ScrollHandler::conclude() {
    active = false;
}
