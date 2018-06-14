#include "Smoother.h"

Smoother::Smoother(double weight) : weight(weight) {}

void Smoother::add(double value) {
    aggregate *= 1 - weight;
    aggregate += value * weight;
}

double Smoother::get() {
    return aggregate;
}
