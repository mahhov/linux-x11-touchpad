#ifndef HELLOWORLDC_TOUCHCONTROLLER_H
#define HELLOWORLDC_TOUCHCONTROLLER_H

#include <fcntl.h>
#include <iostream>
#include <libevdev/libevdev-uinput.h>
#include <libevdev/libevdev.h>
#include <linux/input.h>
#include <poll.h>
#include <unistd.h>
#include <X11/extensions/XTest.h>
#include <X11/Xlib.h>
#include "Touch.h"
#include "Touch.h"
#include "TouchController.h"

typedef input_event InputEvent;
typedef pollfd PollFd;
typedef input_absinfo InputAbsinfo;

class TouchController {
private:
    Display *display;
    Window root;
    int inputFd, outputFd;
    libevdev *inputDevice;
    libevdev_uinput *outputDevice;
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
    void findInputDevice();

    void createOutputDevice();

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
