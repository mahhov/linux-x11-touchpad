#ifndef HELLOWORLDC_SMOOTHER_H
#define HELLOWORLDC_SMOOTHER_H

class Smoother {
private:
    double weight, aggregate;

public:
    Smoother(double weight);

    void add(double value);

    double get();
};

#endif
