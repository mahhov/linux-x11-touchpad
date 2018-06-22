#include "ScrollHandler.h"

ScrollHandler::ScrollHandler(int delta, double boundary, double threshold, double smoothness) :
        delta(delta),
        boundary(boundary),
        threshold(threshold),
        active(false),
        center(Point::invalidPoint),
        smoother(smoothness) {
}

ScrollState ScrollHandler::update(TouchHistory history, TouchController &controller, Paint &paint) {
    bool prevActive = active;

    if (history.getState() == PRESSED)
        init(history.getLastPoint());
    else if (history.getState() == RELEASED)
        conclude(controller);
    iterate(history, controller, paint);

    return ScrollState{smoother.get(), getScrollActivity(prevActive, active)};
}

static Point centerShift = Point::invalidPoint; // todo cleanup
static Smoother smoother1{.5};
static bool line;

static Point base = Point::invalidPoint, doubleBase = Point::invalidPoint;

void ScrollHandler::init(Point movement) {
    if (!(active = movement.x > boundary))
        return;

    centerShift = {-.2, 0};
    center = {movement.x - .2, movement.y}; // todo make const
    smoother.reset();
    accumulator.reset();
    smoother1.reset();
    line = true;
    base = Point::invalidPoint;
    doubleBase = Point::invalidPoint;
}

void ScrollHandler::iterate(TouchHistory history, TouchController &controller, Paint &paint) {
    if (!active)
        return;

    Point lastPoint = history.getLastPoint();

    if (doubleBase.invalid) {
        doubleBase = lastPoint;
        return;
    }

    double minDistance = .005;

    if (base.invalid) {
        if (!(lastPoint - doubleBase) > minDistance)
            base = lastPoint;
        return;
    }

    if (!(lastPoint - base) < minDistance)
        return;

    static double x;

    controller.lockPointerPosition();

    Point last = lastPoint;
    Point movement = last - base;
    Point relativeBase = base - center;
    if (near0(!movement, .003) || near0(!relativeBase, .01))
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
    x += change;

    controller.scroll(accumulator.accumulate(change));

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

    doubleBase = base;
    base = lastPoint;
}

void ScrollHandler::conclude(TouchController &controller) {
    active = false;
    controller.unlockPointerPosition();
}

ScrollActivity ScrollHandler::getScrollActivity(bool prevActive, bool active) {
    if (prevActive)
        return active ? ScrollActivity::SCROLLING : ScrollActivity::STOPPED_SCROLLING;
    return active ? ScrollActivity::STARTED_SCROLLING : ScrollActivity::NOT_SCROLLING;
}

bool ScrollHandler::near0(double value, double threshold) {
    return value < threshold && value > -threshold;
}
