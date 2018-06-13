
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

        Point relativeBase = base - center; // todo extract
        double relativeBaseMag = !relativeBase;
        if (!relativeBaseMag)
            return;

//        double deltaMag = !delta;
//        if (!deltaMag)
//            return;

        double dot = relativeBase % delta - .0003; // todo make constant
        double cross = relativeBase * delta;

        Point centerShift = dot > 0 ^ cross > 0 ? ++delta : --delta;
        center = last + centerShift / !centerShift * .2;

        double change = -cross / relativeBaseMag;

        paint.addPoint({.1, scale(change, 1)});
        paint.addPoint({.1, scale(0, 1)});

        paint.addPoint(last);
        Point fakeBase = last - delta * .1 / !delta;
        paint.addPoint(fakeBase);
        paint.addPoint(center);

        // todo support slower scrolling speed based on finger movement
        // todo support for very fast finger getting sparse data
        // todo clean up
        // todo stop mosue from moving while scroll is active
//        controller.scroll(intChange);
        if (scrollFraction != scrollFraction)
            scrollFraction = 0;
        scrollFraction += change * 10;
        int scrollWhole = scrollFraction;
//        controller.scroll(scrollWhole);
        scrollFraction -= scrollWhole;

        printf("%f ; %f = %f / %f\n", scrollFraction, change, cross, relativeBaseMag); // todo why nan sometimes
        for (int a = 1; a < 20; a++)
            paint.addPoint(history.getPastPoint(a));

    }
    paint.addPoint({boundary, 0});
    paint.addPoint({boundary, 1});
}

void ScrollHandler::conclude() {
    active = false;
}
