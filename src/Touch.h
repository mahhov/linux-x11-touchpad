#ifndef HELLOWORLDC_TOUCH_H
#define HELLOWORLDC_TOUCH_H

struct Touch {
    int rawX, rawY;
    double x, y;
    bool touchDown;
};

#endif
