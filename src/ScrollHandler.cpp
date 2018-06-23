#include <cmath>
#include "ScrollHandler.h"

static Point centerShift = Point::invalidPoint; // todo cleanup
static Smoother curvatureSmoother{.5};
static bool line;
static double x;
static Point base = Point::invalidPoint, doubleBase = Point::invalidPoint;
static Smoother smootherLastScroll{.13};

static Smoother alcs{.1};
static Smoother accs{.1};
static Smoother acs{.1};

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

    return ScrollState{smootherLastScroll.get(), getScrollActivity(prevActive, active)};
}

void ScrollHandler::init(Point movement) {
    if (!(active = movement.x > boundary))
        return;

    centerShift = {-.2, 0};
    center = {movement.x - .2, movement.y}; // todo make const
    smoother.reset();
    smootherLastScroll.reset();
    accumulator.reset();
    curvatureSmoother.reset();
    line = true;
    base = Point::invalidPoint;
    doubleBase = Point::invalidPoint;
    alcs.reset();
    accs.reset();
    acs.reset();
}

void ScrollHandler::iterate(TouchHistory history, TouchController &controller, Paint &paint) {
    if (!active)
        return;

    controller.lockPointerPosition();

    Point last = history.getLastPoint();
    Point lastRelBase = last - base;
    Point baseRelDoubleBase = base - doubleBase;
    Point lastRelDoubleBase = last - doubleBase;
    Point baseRelCenter = base - center;
    Point doubleBaseRelCenter = doubleBase - center;

    if (doubleBase.invalid) {
        doubleBase = last;
        return;
    }

    double minDistance = .005, minDistanceSq = minDistance * minDistance;

    if (base.invalid) {
        if (!(last - doubleBase) > minDistance)
            base = last;
        return;
    }

    if (~lastRelBase < minDistanceSq) {
        smootherLastScroll.smooth(0);
        return;
    }

    double cross = lastRelBase * baseRelDoubleBase / !lastRelBase / !baseRelDoubleBase;
    double curvature = fabs(cross / fmax(!lastRelDoubleBase, .00001));

    curvature = curvatureSmoother.smooth(curvature, .1);

    // todo change baseRelCenter to doubleBaseRelCenter
    double lineChange = lastRelDoubleBase * doubleBaseRelCenter / !doubleBaseRelCenter * 12.5;
    double circleChange = cross * 4.25;
    double absLineChange = fabs(lineChange / !lastRelDoubleBase / 40);
    double absCircleChange = fabs(circleChange);

    absLineChange = alcs.smooth(absLineChange);
    absCircleChange = accs.smooth(absCircleChange);

    double s = 1.5;

    if (line && absCircleChange > absLineChange * s)
        line = false;
    else if (!line && absLineChange > absCircleChange * s)
        line = true;

    double change;
    if (!line) {
        change = smoother.smooth(circleChange);
        centerShift = change < 0 ? ++lastRelDoubleBase : --lastRelDoubleBase;
        centerShift = centerShift / !centerShift * .2;
    } else
        change = smoother.smooth(lineChange);

    center = base + centerShift;

    double min = .15;
    if (change > min)
        change = (change - min) * 2 + min;
    else if (change < -min)
        change = (change + min) * 2 - min;

    // todo adjust the change transform

    smootherLastScroll.add(change);
    x += change;

    controller.scroll(accumulator.accumulate(change));

    if (x > 50)
        x -= 100;
    if (x < -50)
        x += 100;

    paint.addPoint(base);
    paint.addPoint(doubleBase);
    paint.addPoint(last);
    paint.addPoint(last - lastRelBase * .1 / !lastRelBase); // base tail
    paint.addPoint(center);

    paint.addPoint({.08, Paint::scale(change, .3)});
    paint.addPoint({.15, Paint::scale(curvature, 1. / 80)});
    paint.addPoint({.28, Paint::scale(x, .01)});
    paint.addPoint({0, Paint::scale(0, 1)});
    paint.addPoint({boundary, 0});
    paint.addPoint({boundary, 1});

    paint.addPoint({.35, Paint::scale(lineChange, .25)});
    paint.addPoint({.38, Paint::scale(circleChange, .25)});
    paint.addPoint({.45, Paint::scale(absLineChange, .25)});
    paint.addPoint({.48, Paint::scale(absCircleChange, .25)});

    doubleBase = base;
    base = last;
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
