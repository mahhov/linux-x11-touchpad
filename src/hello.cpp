#include <thread>
#include "TouchController.h"
#include "ScrollHandler.h"

// void sleep(int milli) {
//     std::this_thread::sleep_for(std::chrono::milliseconds(milli));
// }

//int main(int argc, char *argv[]) {
//    Paint paint{};
//    paint.init(400, 5);
//
//    TouchHistory history{1000};
//    TouchController controller{};
//
//    ScrollHandler scrollHandler{2, .91, .004, .13};
//    GlideHandler glideHandler{3, 700, .9, 1, 50};
//    ScrollGlideHandler scrollGlideHandler{2, .95, .1, .3};
//
//    while (true) {
//        paint.removeAllPoints();
//
//        controller.update();
//        history.update(controller.getTouch());
//        ScrollState scrollState = scrollHandler.update(history, controller, paint);
//        if (scrollState.activity == ScrollActivity::STARTED_SCROLLING || scrollState.activity == ScrollActivity::NOT_SCROLLING)
//            glideHandler.update(history, controller);
//        if (scrollState.activity != ScrollActivity::SCROLLING)
//            scrollGlideHandler.update(history, controller, scrollState);
//
//        paint.repaint();
////        sleep(2);
//    }
//}

#include <linux/uinput.h>

void emit(int fd, int type, int code, int val) {
    struct input_event ie;

    ie.type = type;
    ie.code = code;
    ie.value = val;

    write(fd, &ie, sizeof(ie));
}

int main() {
    struct uinput_setup usetup;
    memset(&usetup, 0, sizeof(usetup));
    strcpy(usetup.name, "Example device");

    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);

    ioctl(fd, UI_SET_EVBIT, EV_REL);
    ioctl(fd, UI_SET_RELBIT, REL_X);
    ioctl(fd, UI_SET_RELBIT, REL_Y);
    ioctl(fd, UI_SET_RELBIT, REL_WHEEL);

    ioctl(fd, UI_DEV_SETUP, &usetup);
    ioctl(fd, UI_DEV_CREATE);

    usleep(300 * 1000);

    emit(fd, EV_REL, REL_WHEEL, 0);
    emit(fd, EV_SYN, SYN_REPORT, 0);
    usleep(10 * 1000);

    for (int i = 0; i < 1; i++) {
        emit(fd, EV_REL, REL_WHEEL, 1);
        emit(fd, EV_SYN, SYN_REPORT, 0);
        usleep(10 * 1000);
    }

    usleep(300 * 1000);
    printf("done\n");
    ioctl(fd, UI_DEV_DESTROY);
    close(fd);
}

// todo change all object paramters to be by reference
// todo use uinput for smaller scroll outputs
