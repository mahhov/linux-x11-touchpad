#include "GlideHandler.h"

GlideHandler::GlideHandler(int delta, double scale, double friction, double minimum) {
    this->delta = delta;
    this->scale = scale;
    this->friction = friction;
    this->minimum = minimum;
}

void GlideHandler::update(TouchHistory history, TouchController &controller) {
    if (history.getState() == RELEASED)
        init(history.getMovement(delta));
    else if (history.getState() == PRESSED)
        conclude();
    iterate(controller);
}

void GlideHandler::init(Point movement) {
    if (active = !movement.invalid) {
        x = movement.x * scale;
        y = movement.y * scale;
    }
}

void GlideHandler::iterate(TouchController &controller) {
    if (active) {
        x *= friction;
        y *= friction;
        active = x * x + y * y > minimum;
        controller.movePointerPosition(x, y);
    }
}

void GlideHandler::conclude() {
    active = false;
}
