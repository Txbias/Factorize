#pragma once

#include <set>

#include "big_int.h"

class Number {

public:
    BigInt originalValue;

    explicit Number(const BigInt& value) {
        this->originalValue = value;
        this->value = value;
    }

    void addFactor(const BigInt& factor) {
        factors.insert(factor);
        this->value /= factor;
    }

    [[nodiscard]] std::multiset<BigInt> getFactors() const {
        return factors;
    }

    [[nodiscard]] BigInt getCurrentValue() const {
        return value;
    }

private:
    BigInt value;
    std::multiset<BigInt> factors;

};

