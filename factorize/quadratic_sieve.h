#pragma once
#include <functional>
#include <set>
#include <vector>

#include "big_int.h"

//typedef BigInt (* polynomialFunc)(const BigInt &number, const BigInt &input);
using polynomialFunc = std::function<BigInt(const BigInt&, const BigInt&)>;
using polynomialCoefficients = std::pair<BigInt, BigInt>;


BigInt basicPolynomial(const BigInt& number, const BigInt& input);

std::vector<std::pair<polynomialFunc, polynomialCoefficients>> generatePolynomials(int amount, const BigInt &number);

bool isQuadraticResidue(const BigInt& number, const BigInt& prime);

BigInt tonelliShanks(const BigInt& number, const BigInt& prime);

std::vector<std::pair<BigInt, BigInt>> sievePolynomial(const std::pair<polynomialFunc, polynomialCoefficients> &polynomial,
                                                       const std::vector<BigInt> &factorBase,
                                                       const BigInt &number);

std::vector<int> computeFactors(BigInt number, const std::vector<BigInt> &factorBase);

std::set<int> computeLinearDependency(const std::vector<std::vector<int>> &factorizationExponents);

std::pair<BigInt, BigInt> computeSquareCongruence(const std::set<int> &square,
                     const std::vector<std::vector<int>> &factorizationExponents,
                     const std::vector<BigInt> &factorBase,
                     const std::vector<std::pair<BigInt, BigInt>> &equivPairs);
