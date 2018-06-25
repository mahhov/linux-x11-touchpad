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

    int edevr = libevdev_new_from_fd(fd, &evdev);
    if (edevr != 0)
        printf("unable to open libevdev pointer\n");

    eventSize = sizeof(InputEvent);

    pollyFd.fd = fd;
    pollyFd.events = POLLIN;

    minX = libevdev_get_abs_minimum(evdev, ABS_X);
    maxX = libevdev_get_abs_maximum(evdev, ABS_X);
    minY = libevdev_get_abs_minimum(evdev, ABS_Y);
    maxY = libevdev_get_abs_maximum(evdev, ABS_Y);

    touch = {};
}

TouchController::~TouchController() {
    XCloseDisplay(display);
    libevdev_free(evdev);
    close(fd);
}

void TouchController::update() {
    while (poll(&pollyFd, 1, 10)) { // todo play with timeout, why do we need it?
        read(fd, &event, eventSize);

        if (event.type == EV_KEY) {
            if (event.code == BTN_TOUCH)
                touch.touchDown = (bool) event.value;
            else if (event.code == BTN_TOOL_DOUBLETAP)
                touch.touchDown = false;

        } else if (event.type == EV_ABS)
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
    while (delta-- > 0) {
        XTestFakeButtonEvent(display, Button4, True, CurrentTime);
        XTestFakeButtonEvent(display, Button4, False, CurrentTime);
    }
    while (++delta < 0) {
        XTestFakeButtonEvent(display, Button5, True, CurrentTime);
        XTestFakeButtonEvent(display, Button5, False, CurrentTime);
    }
    XFlush(display);
}

Touch TouchController::getTouch() {
    return touch;
}

void TouchController::lockPointerPosition() {
    if (pointerLocked)
        return;
    pointerLocked = true;
    libevdev_grab(evdev, LIBEVDEV_GRAB);
}

void TouchController::unlockPointerPosition() {
    if (!pointerLocked)
        return;
    pointerLocked = false;
    libevdev_grab(evdev, LIBEVDEV_UNGRAB);
}
