#include "ScrollHandler.h"

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
    active = movement.x > boundary;
}

void ScrollHandler::iterate(TouchHistory history, TouchController &controller, Paint &paint) {
    if (active) {
        Point center = history.getPastPoint(delta * 10);
        Point base = history.getPastPoint(delta);
        Point last = history.getLastPoint();
        Point delta = last - base;

        Point relativeBase = base - center;
        double relativeBaseMag = !relativeBase;
        if (!relativeBaseMag)
            return;

        double change = relativeBase * delta / relativeBaseMag;

        int intChange = 0; // todo extract
        if (change > threshold)
            intChange = 1;
        else if (change < -threshold)
            intChange = -1;

        Point fakeBase = last - delta * .1 / !delta;
        double fakeChange = change * 50 + .5f;
        if (fakeChange < 0)
            fakeChange = 0;
        if (fakeChange > 1)
            fakeChange = 1;
        paint.addPoint({0, fakeChange});
        paint.addPoint({.05, threshold * 50 + .5});
        paint.addPoint({.05, -threshold * 50 + .5});
        paint.addPoint({.1, intChange * .3 + .5});
        paint.addPoint(last);
        paint.addPoint(fakeBase);
        paint.addPoint(center);

        controller.scroll(intChange);
    }
    paint.addPoint({boundary, 0});
    paint.addPoint({boundary, 1});
}

void ScrollHandler::conclude() {
    active = false;
}
