#include <thread>
#include "TouchController.h"
#include "TouchHistory.h"

void sleep(int milli) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milli));
}

void printTouchState(TouchState touchState) {
    printf("%d %f %f\n", touchState.touchDown, touchState.x, touchState.y);
}

int main(int argc, char *argv[]) {
    TouchHistory history{10};
    TouchController controller;

    while (true) {
        controller.update();
        history.update(controller.getTouchState());
        printTouchState(history.getLastState());
        std::tuple<double, double> avg = history.getAverage();
        std::cout << "AVG:  " << std::get<0>(avg) << ", " << std::get<1>(avg) << "\n";
        sleep(100);
    }
}
