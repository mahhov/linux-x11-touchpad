#include <cmath>
#include "ScrollHandler.h"

static Point centerShift = Point::invalidPoint; // todo cleanup
static bool line;
static Point base = Point::invalidPoint, doubleBase = Point::invalidPoint;
static Smoother lastChangeSmoother{.13};

static Smoother absLineChangeSmoother{.1};
static Smoother absCircleChangeSmoother{.1};

double minDistance = .005, minDistanceSq = minDistance * minDistance;
double lineScale = 14;
double circleScale = 4;
double lineModeChangeResistance = 1.5;
double transformMin = .15;

ScrollHandler::ScrollHandler(int delta, double boundary, double threshold, double smoothness) :
        delta(delta),
        boundary(boundary),
        threshold(threshold),
        active(false),
        center(Point::invalidPoint),
        changeSmoother(smoothness) {
}

ScrollState ScrollHandler::update(TouchHistory history, TouchController &controller, Paint &paint) {
    bool prevActive = active;

    if (history.getState() == PRESSED)
        init(history.getLastPoint());
    else if (history.getState() == RELEASED)
        conclude(controller);
    iterate(history, controller, paint);

    return ScrollState{lastChangeSmoother.get(), getScrollActivity(prevActive, active)};
}

void ScrollHandler::init(Point movement) {
    if (!(active = movement.x > boundary))
        return;

    centerShift = {-.2, 0};
    center = {movement.x - .2, movement.y}; // todo make const
    changeSmoother.reset();
    lastChangeSmoother.reset();
    accumulator.reset();
    line = true;
    base = Point::invalidPoint;
    doubleBase = Point::invalidPoint;
    absLineChangeSmoother.reset();
    absCircleChangeSmoother.reset();
}

void ScrollHandler::iterate(TouchHistory history, TouchController &controller, Paint &paint) {
    if (!active)
        return;

    controller.lockPointerPosition();

    Point last = history.getLastPoint();

    if (doubleBase.invalid) {
        doubleBase = last;
        return;
    }
    if (base.invalid) {
        if (!(last - doubleBase) > minDistance)
            base = last;
        return;
    }
    Point lastRelBase = last - base;
    if (~lastRelBase < minDistanceSq) {
        lastChangeSmoother.add(0);
        return;
    }

    Point baseRelDoubleBase = base - doubleBase;
    Point lastRelDoubleBase = last - doubleBase;
    Point baseRelCenter = base - center;
    Point doubleBaseRelCenter = doubleBase - center;

    double cross = lastRelBase * baseRelDoubleBase / !lastRelBase / !baseRelDoubleBase;
    double lineChange = lastRelDoubleBase * doubleBaseRelCenter / !doubleBaseRelCenter * lineScale;
    double circleChange = cross * circleScale;
    double absLineChange = absLineChangeSmoother.smooth(fabs(lineChange));
    double absCircleChange = absCircleChangeSmoother.smooth(fabs(circleChange));

    if (line)
        line = absCircleChange < absLineChange * lineModeChangeResistance;
    else
        line = absLineChange > absCircleChange * lineModeChangeResistance;

    printf("line %d\n", line);

    double change = changeSmoother.smooth(line ? lineChange : circleChange);

    if (!line && absCircleChange > absLineChange * lineModeChangeResistance) {
        centerShift = change < 0 ? ++lastRelDoubleBase : --lastRelDoubleBase;
        centerShift = centerShift / !centerShift * .2;
    }

    center = base + centerShift;

    // todo use dot with base instead of cross with center for line change

    // todo investiage why small movements start with opposite direction center

    if (change > transformMin) // todo adjust the transform
        change = (change - transformMin) * 2 + transformMin;
    else if (change < -transformMin)
        change = (change + transformMin) * 2 - transformMin;

    lastChangeSmoother.add(change);

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
