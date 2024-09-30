#pragma once


#include "big_int.h"

class Polynomial {
public:
    Polynomial(BigInt a, BigInt b, BigInt number) : a(std::move(a)), b(std::move(b)),
                                                            number(std::move(number)) {}


    BigInt operator()(const BigInt &input) const {
        BigInt res = a*input + b;
        res *= res;
        res -= number;
        return std::move(res);
    }

    const BigInt a, b;

private:
    const BigInt number;
};