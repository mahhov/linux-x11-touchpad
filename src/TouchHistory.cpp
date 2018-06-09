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
        std::cout << "size is: " << history.size() << "\n";

    } else
        state = state == RELEASED || state == UP ? UP : RELEASED;
}

Touch TouchHistory::getLastTouch() {
    return history.front();
}

State TouchHistory::getState() {
    return state;
}

Point TouchHistory::getAverage() {
    if (history.empty())
        return {-1, -1};
    return {sumX / history.size(), sumY / history.size()};
}

Point TouchHistory::getMovement(int delta) {
    Touch base = history[std::min(delta, (int) history.size() - 1)];
    return {getLastTouch().x - base.x, getLastTouch().y - base.y};
}
