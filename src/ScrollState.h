#ifndef HELLOWORLDC_SCROLLSTATE_H
#define HELLOWORLDC_SCROLLSTATE_H

enum class ScrollActivity {
    SCROLLING, NOT_SCROLLING, STARTED_SCROLLING, STOPPED_SCROLLING
};

struct ScrollState {
    double scrollSpeed;
    ScrollActivity activity;
};

#endif
