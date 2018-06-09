#ifndef HELLOWORLDC_TOUCHCONTROLLER_H
#define HELLOWORLDC_TOUCHCONTROLLER_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <X11/Xlib.h>
#include <poll.h>
#include <X11/extensions/XTest.h>
#include "TouchState.h"
#include "TouchController.h"
#include "TouchState.h"

#define EVENT_DEVICE "/dev/input/event5"
#define EVIOCGABS(abs) _IOR('E', 0x40 + (abs), struct input_absinfo)

typedef input_event InputEvent;
typedef pollfd PollFd;
typedef input_absinfo InputAbsinfo;

class TouchController {
private:
    Display *display;
    Window root;
    int fd;
    size_t eventSize;
    PollFd pollyFd;
    InputEvent event;
    int minX, maxX, minY, maxY;
    TouchState touchState;

public:
    TouchController();

    ~TouchController();

    void update();

    void setPointerPosition(int x, int y);

    void scroll(int delta);

    TouchState getTouchState();
};

#endif