#include <thread>
#include "TouchController.h"
#include "TouchHistory.h"
#include "ScrollHandler.h"
#include "GlideHandler.h"
#include "ScrollGlideHandler.h"

void sleep(int milli) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milli));
}

int main(int argc, char *argv[]) {
    Paint paint{};
    paint.init(400, 5);

    TouchHistory history{1000};
    TouchController controller{};

    ScrollHandler scrollHandler{2, .91, .004, .13};
    GlideHandler glideHandler{3, 700, .9, 1, 50};
    ScrollGlideHandler scrollGlideHandler{2, .95, .1, .3};

    while (true) {
        paint.removeAllPoints();

        controller.update();
        history.update(controller.getTouch());
        ScrollState scrollState = scrollHandler.update(history, controller, paint);
        if (scrollState.activity == ScrollActivity::STARTED_SCROLLING || scrollState.activity == ScrollActivity::NOT_SCROLLING)
            glideHandler.update(history, controller);
        if (scrollState.activity != ScrollActivity::SCROLLING)
            scrollGlideHandler.update(history, controller, scrollState);

        paint.repaint();
//        sleep(2);
    }
}

// todo change all object paramters to be by reference
// todo ignore other fingers during multi touch
// todo use uinput for smaller scroll outputs
