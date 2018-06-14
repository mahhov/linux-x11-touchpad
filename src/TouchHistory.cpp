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
            sumX = 0;
            sumY = 0;
        }

        history.push_front(touch);
        sumX += touch.x;
        sumY += touch.y;
        if (history.size() > maxSize) {
            touch = history.back();
            history.pop_back();
            sumX -= touch.x;
            sumY -= touch.y;
        }
//        std::cout << "size is: " << history.size() << "\n"; // todo delete

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
    Touch base = history[std::min(delta, (int) history.size() - 1)];
    return {base.x, base.y};
}

Point TouchHistory::getMovement(int delta) {
    if (history.empty())
        return Point::invalidPoint; // todo check for nad handle this case
    return getLastPoint() - getPastPoint(delta);
}

Point TouchHistory::getAverage() {
    if (history.empty())
        return Point::invalidPoint;
    return {sumX / history.size(), sumY / history.size()};
}
