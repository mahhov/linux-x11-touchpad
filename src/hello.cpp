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
    TouchHistory history{1000};
    TouchController controller;


    double vscale = 700, vf = .9, minv = .2;
    double vx, vy;
    bool gliding = false;

    while (true) {
        controller.update();
        history.update(controller.getTouch());

        if (history.getState() == RELEASED) {
            Point movement = history.getMovement(3);
            if (gliding = movement.x != -1) {
                vx = movement.x * vscale;
                vy = movement.y * vscale;
                printf("velocity: %f, %f\n", movement.x, movement.y);
            }
        } else if (history.getState() == PRESSED)
            gliding = false;

        if (gliding) {
            vx *= vf;
            vy *= vf;
            gliding = vx * vx + vy * vy > minv;
            controller.movePointerPosition(vx, vy);
        }

        sleep(10);
    }
}
