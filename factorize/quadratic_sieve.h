#pragma once

#include <set>
#include <vector>

#include "big_int.h"
#include "polynomial.h"



std::vector<int> computeFactors(BigInt number, const std::vector<BigInt> &factorBase);

std::set<int> computeLinearDependency(const std::vector<std::vector<int>> &factorizationExponents);

std::pair<BigInt, BigInt> computeSquareCongruence(const std::set<int> &square,
                     const std::vector<std::vector<int>> &factorizationExponents,
                     const std::vector<BigInt> &factorBase,
                     const std::vector<std::pair<BigInt, BigInt>> &equivPairs,
                     const BigInt &number);

std::vector<std::pair<BigInt, BigInt>> sievePolynomial(const Polynomial& polynomial,
                                                 const std::vector<std::pair<BigInt, BigInt>> &solutions,
                                                 const std::vector<BigInt> &factorBase,
                                                 long long sieveRange);

BigInt polynomial(const BigInt& a, const BigInt& b, const BigInt &number, const BigInt &input);

std::vector<BigInt> selectBasePrimes(const BigInt &number, std::vector<BigInt> factorBase,
                                     long long sieveRange);
