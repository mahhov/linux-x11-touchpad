#include <thread>
#include "TouchController.h"
#include "TouchHistory.h"

void sleep(int milli) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milli));
}

void printTouch(Touch touch) {
    printf("%d %f %f\n", touch.touchDown, touch.x, touch.y);
}

int main(int argc, char *argv[]) {
    TouchHistory history{10};
    TouchController controller;


    double vx, vy, vf;

    while (true) {
        controller.update();
        history.update(controller.getTouch());

        if (history.getState() == RELEASED) {
//            Touch baseState = history.getState()
            Touch lastState = history.getLastTouch();

        }

        sleep(100);
    }
}
