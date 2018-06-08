#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <X11/Xlib.h>
#include <poll.h>

#include <X11/extensions/XTest.h>
#include <xcb/xcb.h>
#include <X11/Xlib.h>

#include <chrono>
#include <thread>
#include <cstring>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#define EVENT_DEVICE    "/dev/input/event5"
#define EVENT_TYPE      EV_ABS
#define EVENT_CODE_X    ABS_X
#define EVENT_CODE_Y    ABS_Y

typedef input_event InputEvent;
typedef pollfd PollFd;

Display *display;
Window root;
int fd;
size_t eventSize;
InputEvent event;
PollFd pollyFd;

void initialize() {
    display = XOpenDisplay(NULL);
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
}

void uninitialize() {
    XCloseDisplay(display);
    close(fd);
}

void setPointerPosition(int x, int y) {
    XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
    XFlush(display);
}

void scroll(int delta) {
    int button = delta > 0 ? 4 : 5;
    XTestFakeButtonEvent(display, button, True, CurrentTime);
    XTestFakeButtonEvent(display, button, False, CurrentTime);
    XFlush(display);
}

void sleep(int milli) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milli));
}

int main(int argc, char *argv[]) {
    initialize();
    pollyFd.events = POLLIN;

    while (true) {
//        while (poll(&pollyFd, 1, 10)) {
//            printf("read %i\n", pollyFd.revents);
//            read(fd, &event, eventSize);
//            if (event.type == EVENT_TYPE && (event.code == EVENT_CODE_X || event.code == EVENT_CODE_Y))
//                printf("%s = %d\n", event.code == EVENT_CODE_X ? "X" : "Y", event.value);
//            else
//                printf("%d, %d, %d n", event.type, event.code, event.value);
//        }
//
//        setPointerPosition(100, 200);

        int count;
        XGetMotionEvents(display, root, CurrentTime - 1000 - 1000, CurrentTime + 1000 * 1000, &count);
        std::cout << count << "\n";

        sleep(100);
    }

    uninitialize();
}
