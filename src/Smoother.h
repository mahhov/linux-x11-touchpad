#ifndef HELLOWORLDC_SMOOTHER_H
#define HELLOWORLDC_SMOOTHER_H

class Smoother {
private:
    double weight, aggregate;

public:
    explicit Smoother(double weight);

    void reset();

    double smooth(double value);

    double smooth(double value, double fraction);

    void add(double value);

    void add(double value, double fraction);

    double get();
};

#endif
