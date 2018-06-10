#include <thread>
#include "TouchController.h"
#include "TouchHistory.h"
#include "GlideHandler.h"

void sleep(int milli) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milli));
}

void printTouch(Touch touch) {
    printf("%d %f %f\n", touch.touchDown, touch.x, touch.y);
}

int main(int argc, char *argv[]) {
    TouchHistory history{1000};
    TouchController controller{};

    GlideHandler glideHandler{3, 700, .9, .2};

    while (true) {
        controller.update();
        history.update(controller.getTouch());
        glideHandler.update(history, controller);

        sleep(10);
    }
}
