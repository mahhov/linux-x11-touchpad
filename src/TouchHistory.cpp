#include <iostream>
#include "TouchHistory.h"

TouchHistory::TouchHistory(int size) {
    maxSize = size;
}

void TouchHistory::update(Touch touch) {
    if (!touch.touchDown)
        clear();
    else
        add(touch);
}

Touch TouchHistory::getLastTouch() {
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
    Touch touch = history[delta];
    return {touch.x, touch.y};
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

void TouchHistory::add(Touch touch) {
    history.push_front(touch);
    sumX += touch.x;
    sumY += touch.y;
    if (history.size() > maxSize) {
        touch = history.back();
        history.pop_back();
        sumX -= touch.x;
        sumY -= touch.y;
    }
    if (state == PRESSED)
        state = DOWN;
    else if (state == UP || state == RELEASED)
        state = PRESSED;
}