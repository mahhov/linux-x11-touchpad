#include <cmath>
#include "ScrollHandler.h"

static Point centerShift = Point::invalidPoint; // todo cleanup
static Smoother curvatureSmoother{.5};
static bool line;
static Point base = Point::invalidPoint, doubleBase = Point::invalidPoint;
static Smoother smootherLastScroll{.13};

static Smoother alcs{.1};
static Smoother accs{.1};
static Smoother acs{.1};

double minDistance = .005, minDistanceSq = minDistance * minDistance;
double lineScale = 12.5;
double circleScale = 4.25;
double lineModeChangeResistance = 1.5;
double transformMin = .15;

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
    if (base.invalid) {
        if (!(last - doubleBase) > minDistance)
            base = last;
        return;
    }
    if (~lastRelBase < minDistanceSq) {
        smootherLastScroll.add(0);
        return;
    }

    double cross = lastRelBase * baseRelDoubleBase / !lastRelBase / !baseRelDoubleBase;
    double lineChange = lastRelDoubleBase * doubleBaseRelCenter / !doubleBaseRelCenter * lineScale;
    double circleChange = cross * circleScale;
    double absLineChange = alcs.smooth(fabs(lineChange));
    double absCircleChange = accs.smooth(fabs(circleChange));

    if (line)
        line = absCircleChange < absLineChange * lineModeChangeResistance;
    else
        line = absLineChange > absCircleChange * lineModeChangeResistance;

    double change = smoother.smooth(line ? lineChange : circleChange);

    if (!line) {
        centerShift = change < 0 ? ++lastRelDoubleBase : --lastRelDoubleBase;
        centerShift = centerShift / !centerShift * .2;
    }
    center = base + centerShift;

    if (change > transformMin) // todo adjust the transform
        change = (change - transformMin) * 2 + transformMin;
    else if (change < -transformMin)
        change = (change + transformMin) * 2 - transformMin;

    smootherLastScroll.add(change);

    controller.scroll(accumulator.accumulate(change));

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
