#ifndef HELLOWORLDC_TOUCHSTATE_H
#define HELLOWORLDC_TOUCHSTATE_H

struct TouchState {
    int rawX, rawY;
    double x, y;
    bool touchDown;
};

#endif
