#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main() {
    int delta_x = 500, delta_y = 160;
    Display *display = XOpenDisplay(0);
    Window root = DefaultRootWindow(display);
    XWarpPointer(display, None, root, 0, 0, 0, 0, delta_x, delta_y);
    XCloseDisplay(display);
    return 0;
}