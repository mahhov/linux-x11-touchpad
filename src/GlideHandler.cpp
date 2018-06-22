#include "GlideHandler.h"

GlideHandler::GlideHandler(int delta, double scale, double friction, double minimum, double minimumInitial) : delta(delta), scale(scale),
                                                                                                              friction(friction),
                                                                                                              minimum(minimum),
                                                                                                              minimumInitial(minimumInitial),
                                                                                                              velocity(Point::invalidPoint) {}

void GlideHandler::update(TouchHistory history, TouchController &controller) {
    if (history.getState() == RELEASED)
        init(history);
    else if (history.getState() == PRESSED)
        conclude();
    iterate(controller);
}

void GlideHandler::init(TouchHistory history) {
    Point pointDelta = (history.getLastPoint() - history.getPastPoint(delta)) * scale;
    if (active = history.getSize() > delta && ~pointDelta > minimumInitial)
        velocity = pointDelta;
}

void GlideHandler::iterate(TouchController &controller) {
    if (!active)
        return;
    velocity = velocity * friction;
    active = ~velocity > minimum;
    controller.movePointerPosition((int) velocity.x, (int) velocity.y);
}

void GlideHandler::conclude() {
    active = false;
}
