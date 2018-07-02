#include "TouchController.h"

TouchController::TouchController() {
    display = XOpenDisplay(nullptr);
    root = DefaultRootWindow(display);

    if (getuid() != 0)
        fprintf(stderr, "Need root\n");

    findInputDevice();
    createOutputDevice();

    eventSize = sizeof(InputEvent);

    pollyFd.fd = inputFd;
    pollyFd.events = POLLIN;

    maxX = libevdev_get_abs_maximum(inputDevice, ABS_X);
    minX = libevdev_get_abs_minimum(inputDevice, ABS_X);
    minY = libevdev_get_abs_minimum(inputDevice, ABS_Y);
    maxY = libevdev_get_abs_maximum(inputDevice, ABS_Y);

    touch = {};
}

TouchController::~TouchController() {
    XCloseDisplay(display);

    libevdev_free(inputDevice);
    close(inputFd);

    libevdev_uinput_destroy(outputDevice);
    close(outputFd);
}

void TouchController::findInputDevice() {
    const std::string prefix = "/dev/input/event";
    int i = 0;

    while (true) {
        std::string pathString = prefix + std::to_string(i++);
        const char *path = pathString.c_str();

        inputFd = open(path, O_RDONLY); // todo add O_NONBLOCK?
        if (inputFd == -1) {
            fprintf(stderr, "Unable to open event file %d, %s\n", i, path);
            fprintf(stderr, "Unable to find touch device, iterated %d devices\n", i);
            return;
        }
        if (libevdev_new_from_fd(inputFd, &inputDevice) != 0)
            fprintf(stderr, "Unable to open libevdev device from fd %d, device %d\n", inputFd, i);
        if (libevdev_has_event_type(inputDevice, EV_ABS))
            return;
        libevdev_free(inputDevice);
        close(inputFd);
    }
}

void TouchController::createOutputDevice() {
    outputFd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (outputFd == -1) {
        fprintf(stderr, "Unable to open output event file /dev/uinput\n");
        return;
    }

    struct libevdev *seedDevice = libevdev_new();
    libevdev_set_name(seedDevice, "outputDevice");
    libevdev_enable_event_type(seedDevice, EV_REL);
    libevdev_enable_event_code(seedDevice, EV_REL, REL_WHEEL, NULL);

    if (libevdev_uinput_create_from_device(seedDevice, LIBEVDEV_UINPUT_OPEN_MANAGED, &outputDevice) != 0)
        fprintf(stderr, "Unable to open libevdev uinput device from outputFd %d\n", outputFd);

    libevdev_uinput_write_event(outputDevice, EV_REL, REL_WHEEL, 0);
    libevdev_uinput_write_event(outputDevice, EV_SYN, SYN_REPORT, 0);
}

void TouchController::update() {
    while (poll(&pollyFd, 1, 10)) { // todo play with timeout, why do we need it?
        read(inputFd, &event, eventSize);

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

// todo use libinput for moving pointer

void TouchController::scroll(int delta) {
    if (delta == 0)
        return;
    libevdev_uinput_write_event(outputDevice, EV_REL, REL_WHEEL, delta);
    libevdev_uinput_write_event(outputDevice, EV_SYN, SYN_REPORT, 0);;
}

Touch TouchController::getTouch() {
    return touch;
}

void TouchController::lockPointerPosition() {
    libevdev_grab(inputDevice, LIBEVDEV_GRAB);
    // todo pointer jumping on finger down after initial grab
}

void TouchController::unlockPointerPosition() {
    libevdev_grab(inputDevice, LIBEVDEV_UNGRAB);
}
