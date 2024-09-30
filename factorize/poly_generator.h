#pragma once

#include "polynomial.h"
#include "big_int.h"

#include <vector>

class PolyGenerator {

public:

    PolyGenerator(const BigInt &number, const std::vector<BigInt> &basePrimes,
                  const std::vector<BigInt> &factorBase);

    Polynomial next();

    /**
     * Computes the solutions for polynomial(x)=0 (mod p) for all primes in the factor base
     * @param lastSolutions The solutions of the equation for the polynomial generated previously
     * @param polynomial Polynomial to be solved. Needs to be the Polynomial the latest next()
     *                   call returned
     */
    [[nodiscard]] std::vector<BigInt> nextSolutions(const std::vector<BigInt> &lastSolutions,
                                                   const Polynomial &polynomial) const;

    [[nodiscard]] bool hasNext() const;

    std::vector<BigInt> BValues;

private:
    BigInt a, number;
    BigInt b = -1;

    std::vector<std::vector<BigInt>> addFactors;
    std::vector<BigInt> factorBase;

    long long counter = 1;
};
