#include "GlideHandler.h"

GlideHandler::GlideHandler(double scale, double friction, double minimum) {
    this->scale = scale;
    this->friction = friction;
    this->minimum = minimum;
}

void GlideHandler::update(TouchHistory history, TouchController &controller) {
    if (history.getState() == RELEASED)
        init(history.getMovement(3));
    else if (history.getState() == PRESSED)
        conclude();
    iterate(controller);
}

void GlideHandler::init(Point movement) {
    if (active = movement.x != -1) {
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
