#include "TouchController.h"

TouchController::TouchController() {
    display = XOpenDisplay(nullptr);
    root = DefaultRootWindow(display);

    if (getuid() != 0)
        fprintf(stderr, "Need root\n");

    findDevice();

    eventSize = sizeof(InputEvent);

    pollyFd.fd = fd;
    pollyFd.events = POLLIN;

    minX = libevdev_get_abs_minimum(device, ABS_X);
    maxX = libevdev_get_abs_maximum(device, ABS_X);
    minY = libevdev_get_abs_minimum(device, ABS_Y);
    maxY = libevdev_get_abs_maximum(device, ABS_Y);

    touch = {};
}

TouchController::~TouchController() {
    XCloseDisplay(display);
    libevdev_free(device);
    close(fd);
}

void TouchController::findDevice() {
    const std::string prefix = "/dev/input/event";
    int i = 0;

    while (true) {
        std::string pathString = prefix + std::to_string(i++);
        const char *path = pathString.c_str();

        fd = open(path, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "Unable to open event file %d, %s\n", i, path);
            fprintf(stderr, "Unable to find touch device, iterated %d devices\n", i);
            return;
        }
        if (libevdev_new_from_fd(fd, &device) != 0)
            fprintf(stderr, "Unable to open libevdev device from fd %d, device %d\n", fd, i);
        if (libevdev_has_event_type(device, EV_ABS))
            return;
        libevdev_free(device);
        close(fd);
    }
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
    libevdev_grab(device, LIBEVDEV_GRAB);
}

void TouchController::unlockPointerPosition() {
    if (!pointerLocked)
        return;
    pointerLocked = false;
    libevdev_grab(device, LIBEVDEV_UNGRAB);
}
