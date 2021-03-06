#ifndef HELLOWORLDC_TOUCHHISTORY_H
#define HELLOWORLDC_TOUCHHISTORY_H

#include <deque>
#include "Touch.h"
#include "Point.h"

enum State {
    UP, DOWN, RELEASED, PRESSED
};

class TouchHistory {
private:
    int maxSize;
    std::deque<Touch> history;
    State state;

public:
    explicit TouchHistory(int size);

    void update(Touch touch);

    State getState();

    Point getLastPoint();

    Point getPastPoint(int delta);

    int getSize();
};

#endif
