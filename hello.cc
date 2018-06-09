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

struct TouchState {
    int rawX, rawY;
    double x, y;
    bool touchDown;
};

class TouchController {
public:
    TouchController() {
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

    ~TouchController() {
        XCloseDisplay(display);
        close(fd);
    }

    void update() {
        while (poll(&pollyFd, 1, 10)) {
            read(fd, &event, eventSize);

            if (event.type == EV_KEY && event.code == BTN_TOUCH)
                touchState.touchDown = event.value;

            else if (event.type == EV_ABS)
                if (event.code == ABS_X) {
                    touchState.rawX = event.value;
                    touchState.x = 1. * (touchState.rawX - minX) / (maxX - minX);
                } else if (event.code == ABS_Y) {
                    touchState.rawY = event.value;
                    touchState.y = 1. * (touchState.rawY - minY) / (maxY - minY);
                }
        }
    }

    void setPointerPosition(int x, int y) {
        XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
        XFlush(display);
    }

    void scroll(int delta) {
        const unsigned int button = delta > 0 ? Button4 : Button5;
        XTestFakeButtonEvent(display, button, True, CurrentTime);
        XTestFakeButtonEvent(display, button, False, CurrentTime);
        XFlush(display);
    }

    TouchState getTouchState() {
        return touchState;
    }

private:
    Display *display; // todo can we make these constants?
    Window root;
    int fd;
    size_t eventSize;
    PollFd pollyFd;
    InputEvent event;
    int minX, maxX, minY, maxY;
    TouchState touchState;
};

void sleep(int milli) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milli));
}

void printTouchState(TouchState touchState) {
    printf("%d %f %f\n", touchState.touchDown, touchState.x, touchState.y);
}

int main(int argc, char *argv[]) {
    TouchController touchController;

    while (true) {
        touchController.update();
        TouchState touchState = touchController.getTouchState();
        printTouchState(touchState);
        sleep(100);
    }
}

//X: 1266 - 5676
//y: 1094 - 4760