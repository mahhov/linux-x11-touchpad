#include "ScrollHandler.h"

static bool near0(double value, double threshold);

ScrollHandler::ScrollHandler(int delta, double boundary, double threshold, double smoothness) :
        delta(delta),
        boundary(boundary),
        threshold(threshold),
        active(false),
        center(Point::invalidPoint),
        smoother(smoothness) {}

double ScrollHandler::update(TouchHistory history, TouchController &controller, Paint &paint) {
    bool prevActive = active;

    if (history.getState() == PRESSED)
        init(history.getLastPoint());
    else if (history.getState() == RELEASED)
        conclude(controller);
    iterate(history, controller, paint);

    return active ? SCROLLING : !prevActive ? NOT_SCROLLING : lastScroll;
}

static Point centerShift = Point::invalidPoint;
static Smoother smoother1{.05};
static bool line;

void ScrollHandler::init(Point movement) {
    if (!(active = movement.x > boundary))
        return;

    centerShift = {-.2, 0};
    center = {movement.x - .2, movement.y}; // todo make const
    smoother.reset();
    scrollFraction = 0;
    smoother1.reset();
    line = true;
}

void ScrollHandler::iterate(TouchHistory history, TouchController &controller, Paint &paint) {
    if (!active)
        return;

    static double x;

    controller.lockPointerPosition();

    Point doubleBase = history.getPastPoint(delta * 2 + 1);
    if (doubleBase.invalid)
        return;
    Point base = history.getPastPoint(delta);
    Point last = history.getLastPoint();
    Point movement = last - base;
    Point relativeBase = base - center;
    if (near0(!movement, .0001) || near0(!relativeBase, .01))
        return;

    Point vec1 = base - doubleBase;
    Point vec2 = last - base;
    Point vec3 = last - doubleBase;
    double cros = vec2 * vec1 / !vec1 / !vec2;
    double curvature = cros / (!vec3 > .01 ? !vec3 : .01);
    if (curvature != curvature)
        return;
    if (curvature < 0)
        curvature = -curvature;
    curvature = smoother1.smooth(curvature, !vec3 * 15);

    if (line && !near0(curvature, 2.5))
        line = false;
    if (!line && near0(curvature, 1))
        line = true;

    double change;

    if (line)
        change = vec2 * relativeBase / !relativeBase * 20;
    else {
        centerShift = last - doubleBase;
        centerShift = cros < 0 ? ++centerShift : --centerShift;
        centerShift = centerShift / !centerShift * .2;
        change = cros * 2;
    }
    center = base + centerShift;

    double rawChange = change;
    change = smoother.smooth(change);

    scrollFraction += change;
    int scrollWhole = scrollFraction;
    controller.scroll(scrollWhole);
    scrollFraction -= scrollWhole;

    lastScroll = change;

    x += change;
    if (x > 50)
        x -= 100;
    if (x < -50)
        x += 100;

    paint.addPoint(base);
    paint.addPoint(doubleBase);
    paint.addPoint(last);
    paint.addPoint(last - movement * .1 / !movement); // base tail
    paint.addPoint(center);

    paint.addPoint({.1, Paint::scale(rawChange, .3)});
    paint.addPoint({.13, Paint::scale(change, .3)});
    paint.addPoint({.3, Paint::scale(curvature, 1. / 80)});
    paint.addPoint({.2, Paint::scale(x, .01)});
    paint.addPoint({.1, Paint::scale(0, 1)});
    paint.addPoint({boundary, 0});
    paint.addPoint({boundary, 1});

    // todo clean up
    // todo use x12 to do smaller scroll increments
}

void ScrollHandler::conclude(TouchController &controller) {
    active = false;
    controller.unlockPointerPosition();
}

static bool near0(double value, double threshold) {
    return value < threshold && value > -threshold;
}
