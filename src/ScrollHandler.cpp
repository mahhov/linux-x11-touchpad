
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

ScrollHandler::ScrollHandler(int delta, double boundary, double threshold, double smoothness) :
        delta(delta),
        boundary(boundary),
        threshold(threshold),
        active(false),
        center(Point::invalidPoint),
        smoother(smoothness) {}

void ScrollHandler::update(TouchHistory history, TouchController &controller, Paint &paint) {
    if (history.getState() == PRESSED)
        init(history.getLastPoint());
    else if (history.getState() == RELEASED)
        conclude(controller);
    iterate(history, controller, paint);
}

static Point centerShift = Point::invalidPoint;
static Smoother smoother1{.05};
static bool line;
static double tempMax;

void ScrollHandler::init(Point movement) {
    if (active = movement.x > boundary) {
        centerShift = {-.2, 0};
        center = {movement.x - .2, movement.y}; // todo make const
        smoother.reset();
        scrollFraction = 0;
        smoother1.reset();
        line = true;
        tempMax = 0;
    }
}

void ScrollHandler::iterate(TouchHistory history, TouchController &controller, Paint &paint) {
    static double x;
    if (active) {
        controller.lockPointerPosition();

        Point doubleBase = history.getPastPoint(delta * 2 + 1);
        Point base = history.getPastPoint(delta);
        if (base.invalid)
            return;
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
        tempMax = curvature > tempMax ? curvature : tempMax;
        printf("temp max curvature %f fraction %f\n", tempMax, !vec3 * 15);
        double change;

        if (line && !near0(curvature, 2.5))
            line = false;
        if (!line && near0(curvature, 1))
            line = true;

        if (line) {
            change = vec2 * relativeBase / !relativeBase * 20;
            printf("LINE curv %f change %f\n", curvature, change);
        } else {
            centerShift = last - doubleBase;
            centerShift = cros < 0 ? ++centerShift : --centerShift;
            centerShift = centerShift / !centerShift * .2;
            change = cros * 2;
            printf("CIRCLE curv %f change %f\n", curvature, change);
        }
        center = base + centerShift;

        double rawChange = change;
        change = smoother.smooth(change);

        scrollFraction += change;
        int scrollWhole = scrollFraction;
        controller.scroll(scrollWhole);
        scrollFraction -= scrollWhole;

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

        paint.addPoint({.1, scale(rawChange, .3)});
        paint.addPoint({.13, scale(change, .3)});
        paint.addPoint({.3, scale(curvature, 1. / 80)});
    }
    paint.addPoint({.2, scale(x, .01)});
    paint.addPoint({.1, scale(0, 1)});
    paint.addPoint({boundary, 0});
    paint.addPoint({boundary, 1});

    // todo clean up
    // todo use x12 to do smaller scroll increments
    // when small finger movements, scrolls 2 fast and center gets messed up
}

void ScrollHandler::conclude(TouchController &controller) {
    active = false;
    controller.unlockPointerPosition();
}
