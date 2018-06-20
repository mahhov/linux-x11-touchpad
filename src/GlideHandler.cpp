#include "GlideHandler.h"

GlideHandler::GlideHandler(int delta, double scale, double friction, double minimum) : velocity(Point::invalidPoint) {
    this->delta = delta;
    this->scale = scale;
    this->friction = friction;
    this->minimum = minimum;
}

void GlideHandler::update(TouchHistory history, TouchController &controller) {
    if (history.getState() == RELEASED)
        init(history);
    else if (history.getState() == PRESSED)
        conclude();
    iterate(controller);
}

void GlideHandler::init(TouchHistory history) {
    Point pointDelta = (history.getLastPoint() - history.getPastPoint(delta)) * scale;
    if (active = history.getSize() > delta && ~pointDelta > minimum)
        velocity = pointDelta;
}

void GlideHandler::iterate(TouchController &controller) {
    if (active) {
        velocity = velocity * friction;
        active = ~velocity > minimum;
        controller.movePointerPosition(velocity.x, velocity.y);
    }
}

void GlideHandler::conclude() {
    active = false;
}
