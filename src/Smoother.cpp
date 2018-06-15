#include "Smoother.h"

Smoother::Smoother(double weight) : weight(weight) {}

void Smoother::reset() {
    aggregate = 0;
}

double Smoother::smooth(double value) {
    add(value);
    return get();
};

double Smoother::smooth(double value, double fraction) {
    add(value, fraction > 1 ? 1 : fraction);
    return get();
};

void Smoother::add(double value) {
    aggregate *= 1 - weight;
    aggregate += value * weight;
}

void Smoother::add(double value, double fraction) {
    double weightFraction = weight * fraction;
    aggregate *= 1 - weightFraction;
    aggregate += value * weightFraction;
}

double Smoother::get() {
    return aggregate;
}
