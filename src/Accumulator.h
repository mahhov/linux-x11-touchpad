#ifndef HELLOWORLDC_ACCUMULATOR_H
#define HELLOWORLDC_ACCUMULATOR_H

class Accumulator {
private:
    double value;

public:
    void reset();

    int accumulate(double amount);
};

#endif
