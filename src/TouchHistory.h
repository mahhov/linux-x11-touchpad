#ifndef HELLOWORLDC_TOUCHHISTORY_H
#define HELLOWORLDC_TOUCHHISTORY_H

#include <deque>
#include <tuple>
#include "Touch.h"

enum State {
    UP, DOWN, RELEASED, PRESSED
};

class TouchHistory {
private:
    int maxSize;
    std::deque<Touch> history;
    double sumX, sumY;
    State state;

public:
    TouchHistory(int size);

    void update(Touch touch);

    Touch getLastTouch();

    State getState();

    std::tuple<double, double> getAverage();

    std::tuple<double, double> getMovement(int delta);

private:
    void clear();

    void add(Touch touch);
};

#endif
