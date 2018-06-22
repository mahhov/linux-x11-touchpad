#include "ScrollGlideHandler.h"


ScrollGlideHandler::ScrollGlideHandler(double scale, double friction, double minimum, double minimumInitial) : scale(scale),
                                                                                                               friction(friction),
                                                                                                               minimum(minimum),
                                                                                                               minimumInitial(minimumInitial) {}

void ScrollGlideHandler::update(TouchHistory history, TouchController &controller, ScrollState scrollState) {
    if (history.getState() == RELEASED && scrollState.activity == ScrollActivity::STOPPED_SCROLLING)
        init(scrollState.scrollSpeed);
    else if (history.getState() == PRESSED)
        conclude();
    iterate(controller);
}

void ScrollGlideHandler::init(double scrollSpeed) {
    scrollSpeed *= scale;
    if (!(active = scrollSpeed > minimumInitial || -scrollSpeed > minimumInitial))
        return;
    velocity = scrollSpeed;
    accumulator.reset();
}

void ScrollGlideHandler::iterate(TouchController &controller) {
    if (!active)
        return;
    velocity = velocity * friction;
    active = velocity > minimum || -velocity > minimum;; // todo use global near0 in scroll glide handler & glid ehandler
    controller.scroll(accumulator.accumulate(velocity));
}

void ScrollGlideHandler::conclude() {
    active = false;
}
