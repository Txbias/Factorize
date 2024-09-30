#include "poly_generator.h"

#include <cassert>
#include <iostream>

#include "utils.h"


PolyGenerator::PolyGenerator(const BigInt &number, const std::vector<BigInt> &basePrimes,
                             const std::vector<BigInt> &factorBase) {

    this->factorBase = factorBase;

    assert(basePrimes.size() <= 63);
    this->number = number;

    a = basePrimes[0];
    for(int i = 1;i < basePrimes.size();i++) {
        a *= basePrimes[i];
    }

    BValues.resize(basePrimes.size());

    // Initialize BValues
    for(int i = 0; i < basePrimes.size(); i++) {
        BigInt frac = a/basePrimes[i];
        BigInt inv = BigInt::modInverse(frac, basePrimes[i]);

        BigInt t1 = tonelliShanks(number, basePrimes[i]);
        BigInt t2 = (t1 * (BigInt(-1))) + basePrimes[i];

        const BigInt gamma1 = (t1 * inv) % basePrimes[i];
        const BigInt gamma2 = (t2 * inv) % basePrimes[i];

        if(gamma1 > gamma2) {
            BValues[i] = frac * gamma2;
        } else {
            BValues[i] = frac * gamma1;
        }
    }

    // precompute addFactors
    addFactors.resize(factorBase.size(), std::vector<BigInt>(basePrimes.size()));


    for(int i = 0; i < factorBase.size(); i++) {
        const BigInt aInv = BigInt::modInverse(a, factorBase[i]);
        for(int j = 0; j < basePrimes.size(); j++) {
            addFactors[i][j] = BigInt(2) * BValues[j] * aInv;
            addFactors[i][j] %= factorBase[i];
        }
    }

}

Polynomial PolyGenerator::next() {
    if(b == -1) {
        b= 0;
        for(const auto & BValue : BValues) {
            b += BValue;
        }
        b %= a;
        counter = 1;
        return {a, b, number};
    }

    // Find position of the least significant bit set
    const long long mu = __builtin_ctz(counter & -counter);
    // calculates ceil(counter/2^(mu))
    const long long exponent = 1LL + (counter - 1LL)/(1LL<<(mu + 1LL));
    BigInt multiplier = 2;
    // Check if exponent is odd
    if(exponent & 1) {
        multiplier *= -1;
    }

    assert(mu < BValues.size());
    b += multiplier * BValues[mu];

    counter++;
    return {a, b, number};
}


std::vector<BigInt> PolyGenerator::nextSolutions(const std::vector<BigInt> &lastSolutions,
                                                const Polynomial &polynomial) const {

    std::vector<BigInt> solutions(factorBase.size());

    const long long mu = __builtin_ctz((counter - 1) & -(counter - 1));
    // calculates ceil((counter - 1)/2^(mu))
    const long long exponent = 1LL + (counter - 2LL)/(1LL<<(mu + 1LL));

    auto multiplier = BigInt(1);
    if(!(exponent & 1)) {
        multiplier *= -1;
    }

    for(int i = 0; i < factorBase.size(); ++i) {
        if(lastSolutions[i] == BigInt(-1)) {
            // Needs to be solved from scratch
            BigInt sol = tonelliShanks(number, factorBase[i]);
            sol -= polynomial.b;
            sol *= BigInt::modInverse(polynomial.a, factorBase[i]);
            solutions[i] = sol;
            continue;
        }

        BigInt res = addFactors[i][mu] * multiplier;

        res += lastSolutions[i];
        res %= factorBase[i];
        solutions[i] = res;
    }
    return std::move(solutions);
}


bool PolyGenerator::hasNext() const {
    return counter < (1LL<<(BValues.size() - 1));
}



