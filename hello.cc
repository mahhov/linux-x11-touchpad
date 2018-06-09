#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <X11/Xlib.h>
#include <poll.h>

#include <X11/extensions/XTest.h>

#include <chrono>
#include <thread>

#define EVENT_DEVICE "/dev/input/event5"
#define EVIOCGABS(abs) _IOR('E', 0x40 + (abs), struct input_absinfo)

typedef input_event InputEvent;
typedef pollfd PollFd;
typedef input_absinfo InputAbsinfo;

Display *display;
Window root;
int fd;
size_t eventSize;
InputEvent event;
PollFd pollyFd;
int minX, maxX, minY, maxY;
int touchX, touchY;
bool touchDown;

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

    InputAbsinfo absinfo{};
    ioctl(fd, EVIOCGABS(ABS_X), &absinfo);
    minX = absinfo.minimum;
    maxX = absinfo.maximum;
    ioctl(fd, EVIOCGABS(ABS_Y), &absinfo);
    minY = absinfo.minimum;
    maxY = absinfo.maximum;
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

    while (true) {
        while (poll(&pollyFd, 1, 10)) {
            read(fd, &event, eventSize);

            if (event.type == EV_KEY && event.code == BTN_TOUCH)
                touchDown = event.value;

            else if (event.type == EV_ABS)
                if (event.code == ABS_X)
                    touchX = event.value;
                else if (event.code == ABS_Y)
                    touchY = event.value;

            printf("%d %d %d\n", touchDown, touchX, touchY);
        }

        sleep(100);
    }

    uninitialize();
}


//X: 1266 - 5676
//y: 1094 - 4760