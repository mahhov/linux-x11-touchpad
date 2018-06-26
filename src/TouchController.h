#ifndef HELLOWORLDC_TOUCHCONTROLLER_H
#define HELLOWORLDC_TOUCHCONTROLLER_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <X11/Xlib.h>
#include <poll.h>
#include <X11/extensions/XTest.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include "Touch.h"
#include "TouchController.h"
#include "Touch.h"

typedef input_event InputEvent;
typedef pollfd PollFd;
typedef input_absinfo InputAbsinfo;

class TouchController {
private:
    Display *display;
    Window root;
    int fd;
    libevdev *device;
    size_t eventSize;
    PollFd pollyFd;
    InputEvent event;
    int minX, maxX, minY, maxY;
    Touch touch;
    bool pointerLocked;

public:
    TouchController();

    ~TouchController();

private:
    void findDevice();

public:
    void update();

    void movePointerPosition(int x, int y);

    void setPointerPosition(int x, int y);

    void scroll(int delta);

    Touch getTouch();

    void lockPointerPosition();

    void unlockPointerPosition();
};

#endif
