#include "factorize.h"

#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>


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


void runQuadraticSieve(const BigInt& number) {


    const BigInt logn = BigInt::log2(number);
    const BigInt exponent = BigInt::sqrt(logn * BigInt::log2(logn)) / BigInt(2);
    const long long amount = static_cast<long long>(BigInt::exp(3, exponent, 0));

    const std::vector<BigInt> factorBase = generateFactorBase(amount, number);

    std::cout << "Using factor base of size " << factorBase.size() << std::endl;

    auto polynomials = generatePolynomials(50, number);
    //polynomials.emplace_back(basicPolynomial);

    std::set<std::pair<BigInt, BigInt>> equivPairsSet;

    for(const auto &polynomial : polynomials) {
        auto newPairs = sievePolynomial(polynomial, factorBase, number);
        for(auto & newPair : newPairs) {
            equivPairsSet.insert(std::move(newPair));
        }
    }

    std::vector<std::pair<BigInt, BigInt>> equivPairs;
    for(const auto &[fst, snd] : equivPairsSet) {
        equivPairs.emplace_back(fst, snd);
    }

    std::cout << "congruences found: " << equivPairs.size() << std::endl;

    std::vector<std::vector<int>> factorizationExponents(equivPairs.size());
    for(int i = 0; i < equivPairs.size(); ++i) {
        factorizationExponents[i]= computeFactors(equivPairs[i].second, factorBase);
    }


    const auto square = computeLinearDependency(factorizationExponents);



    std::cout << "Attempting to find square congruence" << std::endl;

    auto [first, second] = computeSquareCongruence(square, factorizationExponents,
                                              factorBase, equivPairs);

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


void runFactorization(const BigInt& num) {



    Number number = preprocessNumber(num);

    for(const BigInt fac : number.getFactors()) {
        std::cout << fac << std::endl;
    }
    std::cout << "running pollard rho" << std::endl;
    number = pollardRho(number);

    for(const BigInt fac : number.getFactors()) {
        std::cout << fac << std::endl;
    }

    std::cout << "remaining Number: " << number.getCurrentValue() << std::endl;

    runQuadraticSieve(number.getCurrentValue());


}