#include "Smoother.h"

Smoother::Smoother(double weight) : weight(weight) {}

void Smoother::reset() {
    aggregate = 0;
}

double Smoother::smooth(double value) {
    add(value);
    return get();
};

void Smoother::add(double value) {
    aggregate *= 1 - weight;
    aggregate += value * weight;
}

double Smoother::get() {
    return aggregate;
}
