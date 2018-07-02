#include <cmath>
#include "ScrollHandler.h"

static bool clockwise;
static bool line;
static Point base = Point::invalidPoint, doubleBase = Point::invalidPoint;
static Smoother lastChangeSmoother{.13};

static Smoother absLineChangeSmoother{.1};
static Smoother absCircleChangeSmoother{.1};

double minDistance = .001, minDistanceSq = minDistance * minDistance;
double lineScale = 17;
double circleScale = 15 * lineScale;
double lineModeChangeResistance = 1.5;
double transformMin = .15;

ScrollHandler::ScrollHandler(int delta, double boundary, double threshold, double smoothness) :
        delta(delta),
        boundary(boundary),
        threshold(threshold),
        active(false),
        changeSmoother(smoothness) {
}

ScrollState ScrollHandler::update(TouchHistory history, TouchController &controller, Paint &paint) {
    bool prevActive = active;

    if (history.getState() == PRESSED)
        init(controller, history.getLastPoint());
    else if (history.getState() == RELEASED)
        conclude(controller);
    iterate(history, controller, paint);

    return ScrollState{lastChangeSmoother.get(), getScrollActivity(prevActive, active)};
}

void ScrollHandler::init(TouchController &controller, Point movement) {
    if (!(active = movement.x > boundary))
        return;

    controller.lockPointerPosition();
    changeSmoother.reset();
    lastChangeSmoother.reset();
    accumulator.reset();
    line = true;
    clockwise = true;
    absLineChangeSmoother.reset(2);
    absCircleChangeSmoother.reset();
    doubleBase = movement + Point{0, -minDistance * 2};
    base = movement + Point{0, -minDistance};
}

void ScrollHandler::iterate(TouchHistory history, TouchController &controller, Paint &paint) {
    if (!active)
        return;

    Point last = history.getLastPoint();

    Point lastRelBase = last - base;
    Point baseRelDoubleBase = base - doubleBase;

    if (~lastRelBase < minDistanceSq) {
        lastChangeSmoother.add(0);
        return;
    }

    double lineChange = lastRelBase % baseRelDoubleBase / !baseRelDoubleBase * lineScale;
    if (clockwise)
        lineChange = -lineChange;
    double circleChange = lastRelBase * baseRelDoubleBase / !baseRelDoubleBase * circleScale;
    double absLineChange = absLineChangeSmoother.smooth(fabs(lineChange));
    double absCircleChange = absCircleChangeSmoother.smooth(fabs(circleChange));

    if (line)
        line = absCircleChange < absLineChange * lineModeChangeResistance;
    else
        line = absLineChange > absCircleChange * lineModeChangeResistance;

    double change = changeSmoother.smooth(line ? lineChange : circleChange);

    if (line)
        clockwise = lineChange < 0;
    else
        clockwise = change < 0;

    if (change > transformMin)
        change = (change - transformMin) * 2 + transformMin;
    else if (change < -transformMin)
        change = (change + transformMin) * 2 - transformMin;

    lastChangeSmoother.add(change);

    controller.scroll(accumulator.accumulate(change));

    // PAINTING - FOR DEBUGGING

    paint.addPoint({0, clockwise ? .75 : .25});
    paint.addPoint(last);
    paint.addPoint(base);
    paint.addPoint(doubleBase);

    static double x = 0;
    double xScale = lineScale;
    x += change;
    if (x > xScale)
        x = -xScale;
    if (x < -xScale)
        x = xScale;
    paint.addPoint({.1, paint.scale(x, .5 / xScale)});

    double changeScale = 3;
    paint.addPoint({.3, paint.scale(lineChange, changeScale)});
    paint.addPoint({.325, .5});
    paint.addPoint({line ? .3 : .35, .5});
    paint.addPoint({.35, paint.scale(circleChange, changeScale)});

    paint.addPoint({.4, paint.scale(absLineChange, changeScale)});
    paint.addPoint({.425, .5});
    paint.addPoint({line ? .4 : .45, .5});
    paint.addPoint({.45, paint.scale(absCircleChange, changeScale)});

    // PAINTING END

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
