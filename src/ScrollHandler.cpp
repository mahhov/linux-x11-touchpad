
#include "ScrollHandler.h"

double scale(double value, double factor) {
    double scaled = -value * factor + .5f;
    if (scaled < 0)
        scaled = 0;
    if (scaled > 1)
        scaled = 1;
    return scaled;
} // todo extract

bool near0(double value, double threshold) {
    return value < threshold && value > -threshold;
} // todo extract

ScrollHandler::ScrollHandler(int delta, double boundary, double threshold) :
        delta(delta),
        boundary(boundary),
        threshold(threshold),
        active(false),
        center(Point::invalidPoint) {}

void ScrollHandler::update(TouchHistory history, TouchController &controller, Paint &paint) {
    if (history.getState() == PRESSED)
        init(history.getLastPoint());
    else if (history.getState() == RELEASED)
        conclude();
    iterate(history, controller, paint);
}

void ScrollHandler::init(Point movement) {
    if (active = movement.x > boundary)
        center = {movement.x - .2, movement.y}; // todo make const
}

void ScrollHandler::iterate(TouchHistory history, TouchController &controller, Paint &paint) {
    if (active) {
        Point base = history.getPastPoint(delta);
        if (base.invalid)
            return;
        Point last = history.getLastPoint();
        Point movement = last - base;
        Point relativeBase = base - center;
        if (near0(!relativeBase, .01))
            return;

        double cross = relativeBase * movement;
        double dotShift = (cross < 0 ? -cross : cross) / 4; // todo make constant
        double dot = relativeBase % movement - dotShift;
        if (near0(cross, .002) || near0(!movement, .01))
            return;

        Point centerShift = dot > 0 ^ cross > 0 ? ++movement : --movement;
        center = (base + movement / 2) + centerShift / !centerShift * .2;

        double change = -cross / !relativeBase; // todo make small circles scroll faster

        scrollFraction += change * 10;
        int scrollWhole = scrollFraction;
//        controller.scroll(scrollWhole);
        scrollFraction -= scrollWhole;

        for (int a = 1; a <= delta; a++)
            paint.addPoint(history.getPastPoint(a));
        paint.addPoint({.4, scale(relativeBase % movement, 1 / .001)});
        paint.addPoint({.1, scale(change, 1)});
        paint.addPoint({.1, scale(0, 1)});
        paint.addPoint(last);
        Point fakeBase = last - movement * .1 / !movement;
        paint.addPoint(fakeBase);
        paint.addPoint(center);
    }
    paint.addPoint({boundary, 0});
    paint.addPoint({boundary, 1});

    // todo support slower scrolling speed based on finger movement
    // todo support for very fast finger getting sparse data
    // todo clean up
    // todo stop mouse from moving while scroll is active
    // todo smoothing filter
}

void ScrollHandler::conclude() {
    active = false;
}