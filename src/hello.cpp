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

    GlideHandler glideHandler{3, 700, .9, 50};
    ScrollHandler scrollHandler{2, .9, .004, .13};

    while (true) {
        paint.removeAllPoints();

        controller.update();
        history.update(controller.getTouch());
        glideHandler.update(history, controller);
        scrollHandler.update(history, controller, paint);

        paint.repaint();
        sleep(2);
    }
}

// todo change all object paramters to be by reference
// todo if scrolling, disable mouse gliding at end of scroll
// todo scroll gliding
