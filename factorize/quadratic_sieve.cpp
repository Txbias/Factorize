#include "quadratic_sieve.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <set>
#include <functional>
#include <random>

#include "big_int.h"

#include <vector>



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


std::vector<BigInt> sieveSolution(const BigInt &prime, const BigInt &solution, const long long range,
                   std::vector<BigInt> sieve) {

    const long long multiplier = (-range - static_cast<long long>(solution)) / static_cast<long long>(prime);
    BigInt index = solution + multiplier*prime;
    assert(BigInt::abs(index) <= range);
    while(index < range) {
        sieve[static_cast<long long>(index)+ range] += BigInt::log2(prime);
        index += prime;
    }

    return std::move(sieve);
}


std::vector<std::pair<BigInt, BigInt>> sievePolynomial(const Polynomial& polynomial,
                                                 const std::vector<std::pair<BigInt, BigInt>> &solutions,
                                                 const std::vector<BigInt> &factorBase,
                                                 const long long sieveRange) {

    std::vector<BigInt> sieve(2*sieveRange+1, 0);

    for(int i = 0; i < solutions.size(); ++i) {

        BigInt sol1 = solutions[i].first;
        BigInt sol2 = solutions[i].second;

        if(sol1 == sol2 && sol1 == factorBase[i]) {
            // factorBase[i] is a base prime, skipping
            continue;
        }

        sieve = sieveSolution(factorBase[i], sol1, sieveRange, std::move(sieve));

        if(sol1 != sol2) sieve = sieveSolution(factorBase[i], sol2, sieveRange, std::move(sieve));
    }

    std::vector<std::pair<BigInt, BigInt>> result;

    const BigInt root = BigInt::sqrt(polynomial.number);
    for(int i = 0; i < sieve.size(); ++i) {
        BigInt cutoff = 1;
        if(i != 0) cutoff = BigInt::log2(2*BigInt::abs(i-sieveRange)*root);
        cutoff *= 2;
        cutoff /= 3;
        if(sieve[i] < cutoff) continue;

        auto polyVal = polynomial(i-sieveRange);
        for(int j = 0; j < factorBase.size(); ++j) {
            if((polyVal % factorBase[j]) == 0) {
                polyVal /= factorBase[j];
            }
        }

        if(polyVal == 1) {
            BigInt x = polynomial.a * (i-sieveRange) + polynomial.b;
            BigInt y = x*x - polynomial.number;
            result.emplace_back(x, y);
        }
    }

    return std::move(result);

}

std::vector<BigInt> selectBasePrimes(const BigInt &number, std::vector<BigInt> factorBase, long long sieveRange) {

    BigInt product = 1;
    const BigInt target = BigInt::sqrt(BigInt(2)*number);
    std::vector<BigInt> basePrimes;

    static auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::default_random_engine rng(seed);

    std::shuffle(factorBase.begin(), factorBase.end(), rng);

    for(int i = 0; i < factorBase.size(); ++i) {
        if(factorBase[i] < 1000 || factorBase[i] > 3000) continue;
        if(product * factorBase[i] <= target) {
            product *= factorBase[i];
            basePrimes.emplace_back(factorBase[i]);
        }
    }
    return std::move(basePrimes);
}
