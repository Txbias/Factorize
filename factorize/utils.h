#pragma once
#include <vector>

#include "big_int.h"


std::vector<BigInt> generatePrimes(long long limit);
std::vector<BigInt> generateFactorBase(long long amount, const BigInt &number);

bool isQuadraticResidue(const BigInt& number, const BigInt& prime);

// first 1000 primes
inline std::vector<BigInt> primes1000 = generatePrimes(7920);
