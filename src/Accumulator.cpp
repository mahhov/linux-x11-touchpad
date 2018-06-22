#include "Accumulator.h"

void Accumulator::reset() {
    value = 0;
}

int Accumulator::accumulate(double amount) {
    value += amount;
    int whole = (int) value;
    value -= whole;
    return whole;
}
