#include "quadratic_sieve.h"

#include <cassert>
#include <iostream>
#include <set>
#include <functional>

#include "big_int.h"
#include "utils.h"

#include <vector>




BigInt basicPolynomial(const BigInt& number, const BigInt& input) {
    auto result = BigInt::ceilSqrt(number);
    result += input;
    result *= result;
    result -= number;
    return std::move(result);
}

BigInt polynomial(const BigInt& a, const BigInt& b, const BigInt &number, const BigInt &input) {
    BigInt res = a * input + b;
    res *= res;
    res -= number;
    return std::move(res);
}

std::vector<std::pair<polynomialFunc, polynomialCoefficients>> generatePolynomials(const int amount,
                                                                                   const BigInt &number) {

    std::vector<std::pair<polynomialFunc, polynomialCoefficients>> polynomials;
    polynomials.reserve(amount);

    assert(primes1000.size() == 1000);
    assert(amount <= 1000);

    int primeIndex = 1;
    for(int i = 0; i < amount; ++i) {
        while(!isQuadraticResidue(number, primes1000[primeIndex])) {
            primeIndex++;
            assert(primeIndex < 1000);
        }
        const BigInt& A = primes1000[primeIndex++];
        BigInt B = tonelliShanks(number, A);
        if(B < BigInt(0)) {
            B += A;
        }
        assert(A > 1);
        assert(B > 0);
        B += (BigInt::ceilSqrt(number)/A) * A;
        B += A;
        if(((B*B) % A) != (number % A)) {
            std::cerr << "B is not a square mod A" << std::endl;
        }

        std::pair<polynomialFunc, polynomialCoefficients> poly;
        poly.second.first = A;
        poly.second.second = B;
        poly.first = [A, B](const BigInt &num, const BigInt &input) {
            return polynomial(A, B, num, input);
        };

        polynomials.push_back(std::move(poly));
    }

    return polynomials;
}


/**
 *
 * @return Pairs of values x and y, where x^2=y (mod number)
 */
std::vector<std::pair<BigInt, BigInt>> sievePolynomial(const std::pair<polynomialFunc, polynomialCoefficients> &polynomial,
                                                       const std::vector<BigInt> &factorBase,
                                                       const BigInt &number) {
    constexpr int amount = 50000;

    std::vector<BigInt> sieve(amount);
    std::vector<BigInt> polynomialValues(amount);

    BigInt A = polynomial.second.first;
    BigInt B = polynomial.second.second;

    for(int i  = 0; i < amount; ++i) {
        sieve[i] = polynomial.first(number, i);

        assert(sieve[i] >= 0);
        polynomialValues[i] = sieve[i];
    }

    for(const BigInt &prime : factorBase) {
        BigInt solution = tonelliShanks(number, prime);
        assert(solution >= 0);
        BigInt solution2 = solution * BigInt(-1);
        solution2 += prime;
        solution2 %= prime;

        const BigInt aInv = BigInt::modInverse(A, prime);

        solution = (solution - B) * aInv;
        solution2 = (solution2 - B) * aInv;

        solution += ((1 + BigInt::abs(solution/prime)) * prime);
        solution2 += ((1 + BigInt::abs(solution2/prime)) * prime);

        solution %= prime;
        solution2 %= prime;

        assert(solution >= 0);
        assert(solution >= 0);

        for(auto i = static_cast<long long>(solution); i < sieve.size(); i+=static_cast<long long>(prime)) {
            assert((sieve[i] % prime) == 0);
            sieve[i] /= prime;
        }

        assert(solution != solution2);

        for(auto i = static_cast<long long>(solution2); i < sieve.size(); i+=static_cast<long long>(prime)) {
            assert((sieve[i] % prime) == 0);
            sieve[i] /= prime;
        }
    }

    std::vector<std::pair<BigInt, BigInt>> result;

    const BigInt root = BigInt::sqrt(number);
    for(int i = 0; i < amount; ++i) {
        if(sieve[i] == 1) {

            BigInt x = A*i + B;
            BigInt y = x*x - number;

            result.emplace_back(x, y);
        }
    }

    return std::move(result);
}

/**
 * Computes the prime factorization of a number.
 * @param number Smooth number over the factor base
 * @param factorBase factor base of prime numbers
 * @return Exponents of the prime factorization of number
 */
std::vector<int> computeFactors(BigInt number, const std::vector<BigInt> &factorBase) {

    std::vector<int> exponents(factorBase.size());
    for(int i = 0; i < factorBase.size(); ++i) {
        while((number % factorBase[i]) == 0) {
            number /= factorBase[i];
            exponents[i]++;
        }
    }

    assert(number == 1);
    return std::move(exponents);
}

/**
 *
 * @param factorizationExponents
 * @return List of indices, specifying which numbers need to be multiplied to get a square
 */
std::set<int> computeLinearDependency(const std::vector<std::vector<int>> &factorizationExponents) {

    assert(!factorizationExponents.empty());
    std::vector<std::vector<bool>> basis(factorizationExponents[0].size());
    std::vector<std::vector<bool>> exponents(factorizationExponents.size());

    std::vector<std::set<int>> originalVectorsUsed(basis.size());

    for(int i = 0; i < exponents.size(); ++i) {
        exponents[i].resize(basis.size());
        for(int j = 0; j < basis.size(); ++j) {
            exponents[i][j] = (factorizationExponents[i][j] % 2) != 0;
        }
    }


    for(int i = 0; i < exponents.size(); ++i) {
        bool added = false;
        std::set<int> usedVectors;
        usedVectors.emplace(i);

        for(int j = 0; j < basis.size(); ++j) {
            if(!exponents[i][j]) continue;

            if(basis[j].empty()) {
                basis[j] = exponents[i];
                originalVectorsUsed[j] = usedVectors;
                added = true;
                break;
            }

            for(int k = 0; k < basis.size(); ++k) {
                exponents[i][k] = basis[j][k] ^ exponents[i][k];
            }

            // add originalVectorsUsed[j] to usedVectors
            for(const auto &v : originalVectorsUsed[j]) {
                if(usedVectors.contains(v)) {
                    usedVectors.erase(v);
                } else {
                    usedVectors.emplace(v);
                }
            }

        }

        if(added) continue;

        // Linear dependency has been found
        std::cout << "Linear dependency found" << std::endl;
        return std::move(usedVectors);
    }

    std::cerr << "No linear dependency found" << std::endl;
    return {};
}

std::pair<BigInt, BigInt> computeSquareCongruence(const std::set<int> &square,
                     const std::vector<std::vector<int>> &factorizationExponents,
                     const std::vector<BigInt> &factorBase,
                     const std::vector<std::pair<BigInt, BigInt>> &equivPairs) {

    BigInt square1 = 1;
    std::vector<int> cntExponents(factorBase.size());
    for(const int i : square) {

        square1 *= equivPairs[i].first;

        for(int j = 0; j < factorizationExponents[i].size(); ++j) {
            cntExponents[j] += factorizationExponents[i][j];
        }
    }

    BigInt square2 = 1;
    for(int i = 0; i < cntExponents.size(); ++i) {
        square2 *= BigInt::exp(factorBase[i], cntExponents[i]/2,0);
    }

    if(square1 < square2) {
        std::swap(square1, square2);
    }

    return std::make_pair(std::move(square1), std::move(square2));
}
