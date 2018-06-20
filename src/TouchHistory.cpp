#include <algorithm>
#include <iostream>
#include "TouchHistory.h"

TouchHistory::TouchHistory(int size) {
    maxSize = size;
    state = UP;
}

void TouchHistory::update(Touch touch) {
    if (touch.touchDown) {
        if (state == PRESSED)
            state = DOWN;
        else if (state != DOWN) {
            state = PRESSED;
            history.clear();
        }

        history.push_front(touch);
        if (getSize() > maxSize)
            history.pop_back();

    } else
        state = state == RELEASED || state == UP ? UP : RELEASED;
}

State TouchHistory::getState() {
    return state;
}

Point TouchHistory::getLastPoint() {
    return {history.front().x, history.front().y};
}

Point TouchHistory::getPastPoint(int delta) {
    if (history.empty())
        return Point::invalidPoint; // todo check for and handle this case
    Touch base = history[std::min(delta, getSize() - 1)];
    return {base.x, base.y};
}

int TouchHistory::getSize() {
    return (int) history.size();
}