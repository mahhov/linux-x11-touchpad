#include "TouchController.h"
#include <thread>

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
