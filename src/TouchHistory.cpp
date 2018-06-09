#include <iostream>
#include "TouchHistory.h"

TouchHistory::TouchHistory(int size) {
    maxSize = size;
}

void TouchHistory::update(TouchState touchState) {
    if (!touchState.touchDown)
        clear();
    else
        add(touchState);
}

TouchState TouchHistory::getLastState() {
    return history.front();
}

State TouchHistory::getState() {
    return state;
}

std::tuple<double, double> TouchHistory::getAverage() {
    if (history.empty())
        return {-1, -1};
    return {sumX / history.size(), sumY / history.size()};
}

std::tuple<double, double> TouchHistory::getMovement(int delta) {
    if (delta < history.size())
        return {-1, -1};
    TouchState touchState = history[delta];
    return {touchState.x, touchState.y};
}

void TouchHistory::clear() {
    history.clear();
    sumX = 0;
    sumY = 0;
    if (state == RELEASED)
        state = UP;
    else if (state == DOWN || state == PRESSED)
        state = RELEASED;
}

void TouchHistory::add(TouchState touchState) {
    history.push_front(touchState);
    sumX += touchState.x;
    sumY += touchState.y;
    if (history.size() > maxSize) {
        touchState = history.back();
        history.pop_back();
        sumX -= touchState.x;
        sumY -= touchState.y;
    }
    if (state == PRESSED)
        state = DOWN;
    else if (state == UP || state == RELEASED)
        state = PRESSED;
}