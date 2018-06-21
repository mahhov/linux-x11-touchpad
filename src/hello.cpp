#include <thread>
#include "TouchController.h"
#include "TouchHistory.h"
#include "ScrollHandler.h"
#include "GlideHandler.h"

void sleep(int milli) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milli));
}

void printTouch(Touch touch) {
    printf("%d %f %f\n", touch.touchDown, touch.x, touch.y);
}

int main(int argc, char *argv[]) {
    Paint paint{};
    paint.init(400, 5);

    TouchHistory history{1000};
    TouchController controller{};

    ScrollHandler scrollHandler{2, .9, .004, .13};
    GlideHandler glideHandler{3, 700, .9, 1, 50};
//    ScrollGlideHandler scrollGlideHandler();

    while (true) {
        paint.removeAllPoints();

        controller.update();
        history.update(controller.getTouch());
        double scrollStatus = scrollHandler.update(history, controller, paint);
        if (scrollStatus == NOT_SCROLLING)
            glideHandler.update(history, controller);
//        if (scrollStatus != SCROLLING)
//            scrollGlideHandler.update(scrollStatus, controller);

        paint.repaint();
        sleep(2);
    }
}

// todo change all object paramters to be by reference
// todo scroll gliding
