#include "factorize.h"

#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>


#include "poly_generator.h"
#include "utils.h"
#include "quadratic_sieve.h"




Number preprocessNumber(const BigInt& num) {

    Number number(num);

    // Check for trivial factors
    for(int i = 0; i < 100; ++i) {
        while((number.getCurrentValue() % primes1000[i]) == 0) {
            number.addFactor(primes1000[i]);
        }
    }


    return number;
}


Number pollardRho(Number number) {
    BigInt x = 2;
    BigInt y = 2;

    BigInt d = 1;

    while(d == 1) {
        x = (x*x + 1) % number.getCurrentValue();
        y = (y*y + 1) % number.getCurrentValue();
        y = (y*y + 1) % number.getCurrentValue();

        d = BigInt::gcd(BigInt::abs(x - y), number.getCurrentValue());
    }

    if(d == number.getCurrentValue()) {
        return number;
    }

    number.addFactor(d);
    return number;
}


void runFactorization(const BigInt &number) {
    constexpr long long sieveRange = 15000;

    const BigInt logn = BigInt::log2(number);
    const BigInt exponent = BigInt::sqrt(logn * BigInt::log2(logn)) / BigInt(2);
    const auto amount = static_cast<long long>(BigInt::exp(2, exponent, 0));

    std::vector<BigInt> factorBase = generateFactorBase(amount*2, number);

    std::cout << "Using factor base of size: " << factorBase.size() << std::endl;

    std::set<std::pair<BigInt, BigInt>> equivPairs;
    while(equivPairs.size() < factorBase.size()) {
        std::vector<BigInt> basePrimes = selectBasePrimes(number, factorBase, sieveRange);

        std::sort(basePrimes.begin(), basePrimes.end());

        std::cout << "base primes: " << std::endl;
        for(const auto& prime : basePrimes) {
            std::cout << "bp: " << prime << std::endl;
        }

        PolyGenerator generator(number, basePrimes, factorBase);

        std::vector<std::pair<BigInt, BigInt>> lastSolutions;


        while(generator.hasNext()) {
            Polynomial polynomial = generator.next();

            assert(((polynomial.b*polynomial.b) % polynomial.a) == (number % polynomial.a));

            std::vector<std::pair<BigInt, BigInt>> solutions = generator.findSolutions(lastSolutions, polynomial);

            auto newEquivPairs = sievePolynomial(polynomial, solutions, factorBase, sieveRange);
            equivPairs.insert(newEquivPairs.begin(), newEquivPairs.end());

            std::cout << "found " << equivPairs.size() << " congruences" << std::endl;

            if(equivPairs.size() > factorBase.size()) {
                std::cout << "found enough congruences, exiting..." << std::endl;
                break;
            }

            lastSolutions = std::move(solutions);
        }
    }

    if(equivPairs.empty()) {
        std::cout << "No solutions found" << std::endl;
        return;
    }

    std::vector<std::vector<int>> factorizationExponents;
    std::vector<std::pair<BigInt, BigInt>> equivPairsVector;
    for(const auto &pair : equivPairs) {
        factorizationExponents.emplace_back(computeFactors(pair.second, factorBase));
        equivPairsVector.emplace_back(pair);
    }


    const auto square = computeLinearDependency(factorizationExponents);



    std::cout << "Attempting to find square congruence" << std::endl;

    auto [first, second] = computeSquareCongruence(square, factorizationExponents,
                                              factorBase, equivPairsVector);

    auto a = first * first;
    a %= number;

    auto b = second * second;
    b %= number;

    if(a != b) {
        std::cerr << "squares not equal" << std::endl;
    }

    BigInt factor = BigInt::gcd(first - second, number);
    BigInt factor2 = BigInt::gcd(first + second, number);

    std::cout << "factor1: " << factor << std::endl;
    std::cout << "factor2: " << factor2 << std::endl;

    auto res = number / factor;
    auto res2 = number / factor2;
    if(res * factor == number && res2 * factor2 == number) {
        std::cout << "factors verified" << std::endl;
    }

}