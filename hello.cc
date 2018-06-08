#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <X11/Xlib.h>
#include <poll.h>

#include <X11/extensions/XTest.h>


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
    int ignored;
    unsigned int ignoredu;

//    XEvent event{};
//    event.type = ButtonPress;
//    event.xbutton.button = Button5;
//
//    printf("start root: %d\n", root);
//
//    XQueryPointer(display, root,
//                  &event.xbutton.root, &event.xbutton.window, &ignored,
//                  &ignored, &event.xbutton.x, &event.xbutton.y,
//                  &ignoredu);
//    event.xbutton.subwindow = event.xbutton.window;
//
//    while (event.xbutton.subwindow) {
//        printf("%d, %d\n", event.xbutton.subwindow, event.xbutton.window);
//        event.xbutton.window = event.xbutton.subwindow;
//        XQueryPointer(display, event.xbutton.window, &event.xbutton.root,
//                      &event.xbutton.subwindow, &ignored, &ignored,
//                      &ignored, &ignored, &ignoredu);
//    }
//    printf("%d, %d\n", event.xbutton.subwindow, event.xbutton.window);
//
//
//    printf("end send %d, %d\n", &event.xbutton.subwindow, event.xbutton.window);

//    XSendEvent(display, PointerWindow, False, ButtonPressMask, &event);

    XTestFakeButtonEvent(display, 4, True, CurrentTime);
    XTestFakeButtonEvent(display, 4, False, CurrentTime);

    //    XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);


    XFlush(display);
}

void sleep(int milli) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milli));
}

int main(int argc, char *argv[]) {
    initialize();
    pollyFd.events = POLLIN;

    while (true) {
        while (poll(&pollyFd, 1, 10)) {
            printf("read %i\n", pollyFd.revents);
            read(fd, &event, eventSize);
            if (event.type == EVENT_TYPE &&
                (event.code == EVENT_CODE_X || event.code == EVENT_CODE_Y)) {
//                printf("%s = %d\n", event.code == EVENT_CODE_X ? "X" : "Y", event.value);
            } else;
//                printf("%d, %d, %d n", event.type, event.code, event.value);
        }

        setPointerPosition(100, 200);

        sleep(100);
    }

    uninitialize();
}
