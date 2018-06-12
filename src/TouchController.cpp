#include "TouchController.h"

TouchController::TouchController() {
    display = XOpenDisplay(nullptr);
    root = DefaultRootWindow(display);

    if (getuid() != 0)
        fprintf(stderr, "You are not root! This may not work...\n");

    fd = open(EVENT_DEVICE, O_RDONLY);
    fprintf(stderr, "reading %s\n", EVENT_DEVICE);

    if (fd == -1)
        fprintf(stderr, "%s is not a vaild device\n", EVENT_DEVICE);

    eventSize = sizeof(InputEvent);

    pollyFd.fd = fd;
    pollyFd.events = POLLIN;

    InputAbsinfo absinfo{};
    ioctl(fd, EVIOCGABS(ABS_X), &absinfo);
    minX = absinfo.minimum;
    maxX = absinfo.maximum;
    ioctl(fd, EVIOCGABS(ABS_Y), &absinfo);
    minY = absinfo.minimum;
    maxY = absinfo.maximum;

    touch = {};
}

TouchController::~TouchController() {
    XCloseDisplay(display);
    close(fd);
}

void TouchController::update() {
    while (poll(&pollyFd, 1, 10)) {
        read(fd, &event, eventSize);

        if (event.type == EV_KEY && event.code == BTN_TOUCH)
            touch.touchDown = (bool) event.value;

        else if (event.type == EV_ABS)
            if (event.code == ABS_X) {
                touch.rawX = event.value;
                touch.x = 1. * (touch.rawX - minX) / (maxX - minX);
            } else if (event.code == ABS_Y) {
                touch.rawY = event.value;
                touch.y = 1. * (touch.rawY - minY) / (maxY - minY);
            }
    }
}

void TouchController::movePointerPosition(int x, int y) {
    XWarpPointer(display, None, None, 0, 0, 0, 0, x, y);
    XFlush(display);
}

void TouchController::setPointerPosition(int x, int y) {
    XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
    XFlush(display);
}

void TouchController::scroll(int delta) {
    if (delta == 0)
        return;
    const unsigned int button = delta < 0 ? Button4 : Button5;
    XTestFakeButtonEvent(display, button, True, CurrentTime);
    XTestFakeButtonEvent(display, button, False, CurrentTime);
    XFlush(display);
}

Touch TouchController::getTouch() {
    return touch;
}
