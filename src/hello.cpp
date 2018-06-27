#include <thread>
#include "TouchController.h"

// todo shouldn't relay on std includes from these headers

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

#include <libevdev/libevdev-uinput.h>

int main() {
    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    struct libevdev_uinput *udevice;

    struct libevdev *seedDevice = libevdev_new();
    libevdev_set_name(seedDevice, "test device");
    libevdev_enable_event_type(seedDevice, EV_REL);
    libevdev_enable_event_code(seedDevice, EV_REL, REL_X, NULL);
    libevdev_enable_event_code(seedDevice, EV_REL, REL_Y, NULL);
    libevdev_enable_event_code(seedDevice, EV_REL, REL_WHEEL, NULL);
    libevdev_enable_event_type(seedDevice, EV_KEY);
    libevdev_enable_event_code(seedDevice, EV_KEY, BTN_LEFT, NULL);

    if (libevdev_uinput_create_from_device(seedDevice, LIBEVDEV_UINPUT_OPEN_MANAGED, &udevice) != 0)
        printf("oh no\n");

    usleep(1000 * 1000);

    libevdev_uinput_write_event(udevice, EV_REL, REL_WHEEL, 0);
    libevdev_uinput_write_event(udevice, EV_SYN, SYN_REPORT, 0);
    libevdev_uinput_write_event(udevice, EV_REL, REL_WHEEL, 1);
    libevdev_uinput_write_event(udevice, EV_SYN, SYN_REPORT, 0);

    libevdev_uinput_destroy(udevice);

    close(fd);
}

// todo change all object paramters to be by reference
// todo use uinput for smaller scroll outputs
